// Copyright (c) 2019, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use digest::Digest;
use hhbc_string_utils::{mangle_xhp_id, strip_global_ns};
use naming_special_names_rust::user_attributes;
use oxidized_by_ref::{
    ast_defs::{Abstraction, ClassishKind},
    direct_decl_parser::Decls,
    shallow_decl_defs::{ClassDecl, TypedefDecl},
    typing_defs::{Ty, Ty_, UserAttribute},
};
use serde::ser::SerializeSeq;
use serde::{
    de::{SeqAccess, Visitor},
    Deserializer, Serializer,
};
use serde_derive::{Deserialize, Serialize};
use serde_json::json;
use std::collections::{BTreeMap, BTreeSet};

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub enum TypeKind {
    Class,
    Record,
    Interface,
    Enum,
    Trait,
    TypeAlias,
    Unknown,
    Mixed,
}

pub type StringSet = BTreeSet<String>;
pub type Attributes = BTreeMap<String, Vec<String>>;

#[derive(Debug, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct MethodFacts {
    #[serde(default, skip_serializing_if = "Attributes::is_empty")]
    pub attributes: Attributes,
}

pub type Methods = BTreeMap<String, MethodFacts>;

#[derive(Debug, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct TypeFacts {
    #[serde(default, skip_serializing_if = "StringSet::is_empty")]
    pub base_types: StringSet,

    #[serde(rename = "kindOf")]
    pub kind: TypeKind,

    #[serde(default)]
    pub attributes: Attributes,

    pub flags: isize,

    #[serde(default, skip_serializing_if = "StringSet::is_empty")]
    pub require_extends: StringSet,

    #[serde(default, skip_serializing_if = "StringSet::is_empty")]
    pub require_implements: StringSet,

    #[serde(default, skip_serializing_if = "Methods::is_empty")]
    pub methods: Methods,
}

pub type TypeFactsByName = BTreeMap<String, TypeFacts>;

#[derive(Default, Debug, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct Facts {
    #[serde(
        default,
        skip_serializing_if = "TypeFactsByName::is_empty",
        serialize_with = "types_to_json",
        deserialize_with = "json_to_types"
    )]
    pub types: TypeFactsByName,

    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub functions: Vec<String>,

    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub constants: Vec<String>,

    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub type_aliases: Vec<String>,

    #[serde(default, skip_serializing_if = "Attributes::is_empty")]
    pub file_attributes: Attributes,
}

impl Facts {
    pub fn to_json_value(&self, text: &[u8]) -> serde_json::Value {
        let sha1sum = sha1(text);
        let mut json = json!(&self);
        if let Some(m) = json.as_object_mut() {
            m.insert(String::from("sha1sum"), json!(sha1sum));
        };
        json
    }

    pub fn to_json(&self, pretty: bool, text: &[u8]) -> String {
        let json = self.to_json_value(text);
        if pretty {
            serde_json::to_string_pretty(&json).expect("Could not serialize facts to JSON")
        } else {
            serde_json::to_string(&json).expect("Could not serialize facts to JSON")
        }
    }

    pub fn from_decls(
        decls: &Decls<'_>,
        file_attributes: &[&UserAttribute<'_>],
        disable_xhp_element_mangling: bool,
    ) -> Facts {
        let mut types = TypeFactsByName::new();
        decls.classes().for_each(|(class_name, decl)| {
            let mut name = format(class_name);
            if !disable_xhp_element_mangling && decl.is_xhp {
                // strips the namespace and mangles the class id
                if let Some(id) = name.rsplit('\\').next() {
                    name = id.to_string();
                }
            }
            let type_fact = TypeFacts::of_class_decl(decl);
            add_or_update_classish_decl(name, type_fact, &mut types);
        });
        let mut type_aliases = decls
            .typedefs()
            .filter_map(|(name, decl)| {
                if decl.is_ctx {
                    // ignore context aliases
                    None
                } else {
                    let type_fact = TypeFacts::of_typedef_decl(decl);
                    add_or_update_classish_decl(format(name), type_fact, &mut types);
                    Some(format(name))
                }
            })
            .collect::<Vec<String>>();
        let mut functions = decls
            .funs()
            .filter_map(|(name, _)| {
                let name = format(name);
                if name.eq("__construct") {
                    None
                } else {
                    Some(name)
                }
            })
            .collect::<Vec<String>>();
        let mut constants = decls
            .consts()
            .map(|(name, _)| format(name))
            .collect::<Vec<String>>();

        functions.reverse();
        constants.reverse();
        type_aliases.reverse();

        Facts {
            types,
            functions,
            constants,
            type_aliases,
            file_attributes: to_facts_attributes(file_attributes),
        }
    }
}

pub type Flags = isize;
#[derive(Clone, Copy)]
pub enum Flag {
    Abstract = 1,
    Final = 2,
    MultipleDeclarations = 4,
}

impl Flag {
    pub fn as_flags(&self) -> Flags {
        *self as isize
    }
    pub fn default() -> Flags {
        0
    }
    pub fn is_set(&self, flags: Flags) -> bool {
        (flags & (*self as Flags)) != 0
    }
    pub fn set(self, flags: Flags) -> Flags {
        flags | (self as Flags)
    }
    pub fn combine(flags1: Flags, flags2: Flags) -> Flags {
        flags1 | flags2
    }
}

pub fn sha1(text: &[u8]) -> String {
    let mut digest = sha1::Sha1::new();
    digest.input(text);
    hex::encode(digest.result())
}

// implementation details

/// Serialize the Map<Name, TypeFacts> as a sequence of JSON objects with `name`
/// as one of the fields.
fn types_to_json<S: Serializer>(types_by_name: &TypeFactsByName, s: S) -> Result<S::Ok, S::Error> {
    let mut seq = s.serialize_seq(None)?;
    for (name, types) in types_by_name.iter() {
        // pull the "name" key into the associated json object, then append to list
        let mut types_json = json!(types);
        if let Some(m) = types_json.as_object_mut() {
            m.insert("name".to_owned(), json!(name));
        };

        // possibly skip non-empty attributes/require*, depending on the kind
        if types.skip_attributes() {
            types_json.as_object_mut().map(|m| m.remove("attributes"));
        }
        if types.skip_require_extends() {
            types_json
                .as_object_mut()
                .map(|m| m.remove("requireExtends"));
        }
        if types.skip_require_implements() {
            types_json
                .as_object_mut()
                .map(|m| m.remove("requireImplements"));
        }
        seq.serialize_element(&types_json)?;
    }
    seq.end()
}

/// Deserialize a sequence of TypeFacts with `name` fields as a Map by hoisting
/// the name as the map key.
fn json_to_types<'de, D: Deserializer<'de>>(d: D) -> Result<TypeFactsByName, D::Error> {
    struct TypeFactsSeqVisitor;
    impl<'de> Visitor<'de> for TypeFactsSeqVisitor {
        type Value = TypeFactsByName;
        fn expecting(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            write!(f, "a sequence of TypeFacts")
        }
        fn visit_seq<A: SeqAccess<'de>>(self, mut seq: A) -> Result<Self::Value, A::Error> {
            let mut types = TypeFactsByName::new();
            while let Some(mut v) = seq.next_element::<serde_json::Value>()? {
                let obj = v
                    .as_object_mut()
                    .ok_or_else(|| serde::de::Error::custom("Expected TypeFacts JSON Object"))?;
                let name = obj
                    .remove("name")
                    .ok_or_else(|| serde::de::Error::missing_field("name"))?;
                let name = name
                    .as_str()
                    .ok_or_else(|| serde::de::Error::custom("Expected name JSON String"))?;
                types.insert(
                    name.into(),
                    serde_json::from_value(v).map_err(serde::de::Error::custom)?,
                );
            }
            Ok(types)
        }
    }
    d.deserialize_seq(TypeFactsSeqVisitor)
}

impl TypeFacts {
    fn skip_require_extends(&self) -> bool {
        match self.kind {
            TypeKind::Trait | TypeKind::Interface => false,
            _ => self.require_extends.is_empty(),
        }
    }
    fn skip_require_implements(&self) -> bool {
        match self.kind {
            TypeKind::Trait => false,
            _ => self.require_implements.is_empty(),
        }
    }
    fn skip_attributes(&self) -> bool {
        self.attributes.is_empty()
    }

    fn of_class_decl<'a>(decl: &'a ClassDecl<'a>) -> TypeFacts {
        let ClassDecl {
            kind,
            final_,
            req_implements,
            req_extends,
            uses,
            extends,
            implements,
            user_attributes,
            methods,
            static_methods,
            enum_type,
            ..
        } = decl;

        // Collect base types from uses, extends, and implements
        let mut base_types = StringSet::new();
        uses.iter().for_each(|ty| {
            base_types.insert(extract_type_name(ty));
        });
        extends.iter().for_each(|ty| {
            base_types.insert(extract_type_name(ty));
        });
        implements.iter().for_each(|ty| {
            base_types.insert(extract_type_name(ty));
        });

        // Set flags according to modifiers - abstract, final, static (abstract + final)
        let mut flags = Flags::default();
        let typekind = match kind {
            ClassishKind::Cclass(abstraction) => {
                flags = modifiers_to_flags(flags, *final_, *abstraction);
                TypeKind::Class
            }
            ClassishKind::Cinterface => {
                flags = Flag::Abstract.as_flags();
                TypeKind::Interface
            }
            ClassishKind::Ctrait => {
                flags = Flag::Abstract.as_flags();
                TypeKind::Trait
            }
            ClassishKind::Cenum => {
                if let Some(et) = enum_type {
                    et.includes.iter().for_each(|ty| {
                        base_types.insert(extract_type_name(ty));
                    });
                }
                TypeKind::Enum
            }
            ClassishKind::CenumClass(abstraction) => {
                flags = modifiers_to_flags(flags, *final_, *abstraction);
                TypeKind::Enum
            }
        };

        let check_require = match typekind {
            TypeKind::Interface | TypeKind::Trait => true,
            _ => false,
        };

        // Collect the requires
        let require_extends = req_extends
            .iter()
            .filter_map(|&ty| {
                if check_require {
                    Some(extract_type_name(ty))
                } else {
                    None
                }
            })
            .collect::<StringSet>();
        let require_implements = req_implements
            .iter()
            .filter_map(|&ty| {
                if check_require {
                    Some(extract_type_name(ty))
                } else {
                    None
                }
            })
            .collect::<StringSet>();

        // TODO(T101762617): modify the direct decl parser to
        // preserve the attribute params that facts expects
        let attributes = to_facts_attributes(user_attributes);

        let methods = methods
            .iter()
            .chain(static_methods.iter())
            .filter_map(|m| {
                let attributes = to_facts_attributes(m.attributes);
                // Add this method to the output iff it's
                // decorated with non-builtin attributes
                if attributes.is_empty() {
                    None
                } else {
                    Some((format(m.name.1), MethodFacts { attributes }))
                }
            })
            .collect::<Methods>();

        TypeFacts {
            base_types,
            kind: typekind,
            require_extends,
            flags,
            require_implements,
            attributes,
            methods,
        }
    }

    fn of_typedef_decl<'a>(decl: &'a TypedefDecl<'a>) -> TypeFacts {
        TypeFacts {
            base_types: StringSet::new(),
            kind: TypeKind::TypeAlias,
            attributes: to_facts_attributes(decl.attributes),
            require_extends: StringSet::new(),
            flags: Flags::default(),
            require_implements: StringSet::new(),
            methods: BTreeMap::new(),
        }
    }
}

fn add_or_update_classish_decl(name: String, mut delta: TypeFacts, types: &mut TypeFactsByName) {
    types
        .entry(name)
        .and_modify(|tf| {
            if tf.kind != delta.kind {
                tf.kind = TypeKind::Mixed;
            }
            tf.flags = Flag::MultipleDeclarations.set(tf.flags);
            tf.flags = Flag::combine(tf.flags, delta.flags);
            tf.base_types.append(&mut delta.base_types);
            tf.attributes.append(&mut delta.attributes);
            tf.require_extends.append(&mut delta.require_extends);
            tf.require_implements.append(&mut delta.require_implements);
        })
        .or_insert(delta);
}

// TODO: move to typing_defs_core_impl.rs once completed
fn extract_type_name<'a>(ty: &Ty<'a>) -> String {
    match ty.get_node() {
        Ty_::Tapply(((_, id), _)) => format(*id),
        Ty_::Tgeneric((id, _)) => format(*id),
        _ => unimplemented!("{:?}", ty),
    }
}

fn format<'a>(original_name: &'a str) -> String {
    let unqualified = strip_global_ns(original_name);
    match unqualified.rsplit('\\').next() {
        Some(id) if original_name.starts_with('\\') && id.starts_with(':') => {
            // only mangle already qualified xhp ids - avoid mangling string literals
            // containing an xhp name, for example an attribute param ':foo:bar'
            mangle_xhp_id(id.to_string())
        }
        _ => String::from(unqualified),
    }
}

fn modifiers_to_flags(flags: isize, is_final: bool, abstraction: Abstraction) -> isize {
    let flags = match abstraction {
        Abstraction::Abstract => Flag::Abstract.set(flags),
        Abstraction::Concrete => flags,
    };
    if is_final {
        Flag::Final.set(flags)
    } else {
        flags
    }
}

fn to_facts_attributes<'a>(attributes: &'a [&'a UserAttribute<'a>]) -> Attributes {
    attributes
        .iter()
        .filter_map(|ua| {
            let attr_name = format(ua.name.1);
            if user_attributes::AS_SET.contains(attr_name.as_str()) {
                // skip builtins
                None
            } else {
                Some((
                    attr_name,
                    ua.classname_params
                        .iter()
                        .map(|param| format(*param))
                        .collect::<Vec<String>>(),
                ))
            }
        })
        .collect::<Attributes>()
}

// inline tests (so stuff can remain hidden) - compiled only when tests are run (no overhead)

#[cfg(test)]
mod tests {
    use super::*;
    use pretty_assertions::assert_eq; // make assert_eq print huge diffs more human-readable

    #[test]
    fn type_kind_to_json() {
        assert_eq!(json!(TypeKind::Unknown).to_string(), "\"unknown\"");
        assert_eq!(json!(TypeKind::Interface).to_string(), "\"interface\"");
    }

    #[test]
    fn sha1_some_text() {
        let text = b"some text";
        assert_eq!(
            sha1(text),
            String::from("37aa63c77398d954473262e1a0057c1e632eda77"),
        );
    }

    #[test]
    fn string_set_to_json() {
        let mut ss = StringSet::new();
        ss.insert("foo".into());
        ss.insert("bar".into());
        assert_eq!(
            r#"[
  "bar",
  "foo"
]"#,
            serde_json::to_string_pretty(&ss).unwrap(),
        );
    }

    fn fake_facts() -> Facts {
        let mut types = TypeFactsByName::new();
        let mut base_types = StringSet::new();
        base_types.insert("bt3".into());
        base_types.insert("bt1".into());
        base_types.insert("bt2".into());
        types.insert(
            String::from("include_empty_both_when_trait_kind"),
            TypeFacts {
                kind: TypeKind::Trait,
                attributes: Attributes::new(),
                require_extends: StringSet::new(),
                require_implements: StringSet::new(),
                base_types,
                flags: 6,
                methods: Methods::new(),
            },
        );
        // verify requireImplements and requireExtends are skipped if empty and Class kind
        types.insert(
            String::from("include_empty_neither_when_class_kind"),
            TypeFacts {
                kind: TypeKind::Class,
                attributes: Attributes::new(),
                require_extends: StringSet::new(),
                require_implements: StringSet::new(),
                base_types: StringSet::new(),
                flags: 0,
                methods: Methods::new(),
            },
        );
        // verify only requireImplements is skipped if empty and Interface kind
        types.insert(
            String::from("include_empty_req_extends_when_interface_kind"),
            TypeFacts {
                kind: TypeKind::Interface,
                attributes: Attributes::new(),
                require_extends: StringSet::new(),
                require_implements: StringSet::new(),
                base_types: StringSet::new(),
                flags: 1,
                methods: Methods::new(),
            },
        );
        // verify non-empty require* is included
        types.insert(
            String::from("include_nonempty_always"),
            TypeFacts {
                kind: TypeKind::Unknown,
                attributes: {
                    let mut map = Attributes::new();
                    map.insert("A".into(), vec!["'B'".into()]);
                    map.insert("C".into(), Vec::new());
                    map
                },
                require_extends: {
                    let mut set = StringSet::new();
                    set.insert("extends1".into());
                    set
                },
                require_implements: {
                    let mut set = StringSet::new();
                    set.insert("impl1".into());
                    set
                },
                base_types: StringSet::new(),
                flags: 9,
                methods: Methods::new(),
            },
        );
        types.insert(
            String::from("include_method_attrs"),
            TypeFacts {
                kind: TypeKind::Class,
                attributes: Attributes::new(),
                require_extends: StringSet::new(),
                require_implements: StringSet::new(),
                base_types: StringSet::new(),
                flags: 6,
                methods: vec![
                    (
                        String::from("no_attrs"),
                        MethodFacts {
                            attributes: Attributes::new(),
                        },
                    ),
                    (
                        String::from("one_attr_with_arg"),
                        MethodFacts {
                            attributes: vec![(
                                String::from("attr_with_arg"),
                                vec![String::from("arg")],
                            )]
                            .into_iter()
                            .collect(),
                        },
                    ),
                ]
                .into_iter()
                .collect(),
            },
        );
        Facts {
            constants: vec!["c1".into(), "c2".into()],
            file_attributes: BTreeMap::new(),
            functions: vec![],
            type_aliases: vec!["my_type_alias".into()],
            types,
        }
    }

    #[test]
    fn round_trip_json() -> serde_json::Result<()> {
        let f1 = fake_facts();
        let ugly = f1.to_json(false, b"fake source text");
        let f2 = serde_json::from_str(&ugly)?;
        assert_eq!(f1, f2);
        let pretty = f1.to_json(true, b"fake source text");
        let f2 = serde_json::from_str(&pretty)?;
        assert_eq!(f1, f2);
        Ok(())
    }

    #[test]
    fn to_json() {
        // test to_string_pretty()
        assert_eq!(
            fake_facts().to_json(true, b"some text"),
            r#"{
  "constants": [
    "c1",
    "c2"
  ],
  "sha1sum": "37aa63c77398d954473262e1a0057c1e632eda77",
  "typeAliases": [
    "my_type_alias"
  ],
  "types": [
    {
      "baseTypes": [
        "bt1",
        "bt2",
        "bt3"
      ],
      "flags": 6,
      "kindOf": "trait",
      "name": "include_empty_both_when_trait_kind"
    },
    {
      "flags": 0,
      "kindOf": "class",
      "name": "include_empty_neither_when_class_kind"
    },
    {
      "flags": 1,
      "kindOf": "interface",
      "name": "include_empty_req_extends_when_interface_kind"
    },
    {
      "flags": 6,
      "kindOf": "class",
      "methods": {
        "no_attrs": {},
        "one_attr_with_arg": {
          "attributes": {
            "attr_with_arg": [
              "arg"
            ]
          }
        }
      },
      "name": "include_method_attrs"
    },
    {
      "attributes": {
        "A": [
          "'B'"
        ],
        "C": []
      },
      "flags": 9,
      "kindOf": "unknown",
      "name": "include_nonempty_always",
      "requireExtends": [
        "extends1"
      ],
      "requireImplements": [
        "impl1"
      ]
    }
  ]
}"#,
        )
    }

    #[test]
    fn test_flags() {
        let flags = Flag::default();
        assert!(!Flag::Final.is_set(flags));
        let flags = Flag::Final.set(flags);
        let flags = Flag::Abstract.set(flags);
        assert!(Flag::Final.is_set(flags));
        assert!(Flag::Abstract.is_set(flags));
    }
}
