// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use std::collections::BTreeMap;

use error::Error;
use error::Result;
use hhbc::DictEntry;
use hhbc::TypedValue;
use hhbc_string_utils as string_utils;
use hhvm_types_ffi::ffi::TypeStructureKind;
use naming_special_names_rust::classes;
use naming_special_names_rust::typehints;
use options::Options;
use oxidized::aast;
use oxidized::aast_defs::Hint;
use oxidized::aast_defs::Hint_;
use oxidized::aast_defs::NastShapeInfo;
use oxidized::aast_defs::ShapeFieldInfo;
use oxidized::ast;
use oxidized::ast_defs;
use oxidized::ast_defs::ShapeFieldName;

use super::TypeRefinementInHint;

fn ts_kind(tparams: &[&str], p: &str) -> TypeStructureKind {
    if tparams.contains(&p) {
        TypeStructureKind::T_typevar
    } else {
        match p.to_lowercase().as_str() {
            "hh\\void" => TypeStructureKind::T_void,
            "hh\\int" => TypeStructureKind::T_int,
            "hh\\bool" => TypeStructureKind::T_bool,
            "hh\\float" => TypeStructureKind::T_float,
            "hh\\string" => TypeStructureKind::T_string,
            "hh\\resource" => TypeStructureKind::T_resource,
            "hh\\num" => TypeStructureKind::T_num,
            "hh\\noreturn" => TypeStructureKind::T_noreturn,
            "hh\\arraykey" => TypeStructureKind::T_arraykey,
            "hh\\mixed" => TypeStructureKind::T_mixed,
            "tuple" => TypeStructureKind::T_tuple,
            "$$internal$$fun" => TypeStructureKind::T_fun,
            "_" | "$$internal$$typevar" => TypeStructureKind::T_typevar,
            "shape" => TypeStructureKind::T_shape,
            "class" => TypeStructureKind::T_class,
            "interface" => TypeStructureKind::T_interface,
            "trait" => TypeStructureKind::T_trait,
            "enum" => TypeStructureKind::T_enum,
            "hh\\dict" => TypeStructureKind::T_dict,
            "hh\\vec" => TypeStructureKind::T_vec,
            "hh\\keyset" => TypeStructureKind::T_keyset,
            "hh\\vec_or_dict" => TypeStructureKind::T_vec_or_dict,
            "hh\\nonnull" => TypeStructureKind::T_nonnull,
            "hh\\darray" => TypeStructureKind::T_darray,
            "hh\\varray" => TypeStructureKind::T_varray,
            "hh\\varray_or_darray" => TypeStructureKind::T_varray_or_darray,
            "hh\\anyarray" => TypeStructureKind::T_any_array,
            "hh\\null" => TypeStructureKind::T_null,
            "hh\\nothing" => TypeStructureKind::T_nothing,
            "hh\\dynamic" => TypeStructureKind::T_dynamic,
            "unresolved" => TypeStructureKind::T_unresolved,
            "$$internal$$typeaccess" => TypeStructureKind::T_typeaccess,
            "$$internal$$reifiedtype" => TypeStructureKind::T_reifiedtype,
            _ if p.len() > 4 && p.starts_with("xhp_") => TypeStructureKind::T_xhp,
            _ => TypeStructureKind::T_unresolved,
        }
    }
}

fn is_prim_or_resolved_classname(kind: TypeStructureKind) -> bool {
    matches!(
        kind,
        // primitives
        TypeStructureKind::T_void
            | TypeStructureKind::T_int
            | TypeStructureKind::T_bool
            | TypeStructureKind::T_float
            | TypeStructureKind::T_string
            | TypeStructureKind::T_resource
            | TypeStructureKind::T_num
            | TypeStructureKind::T_noreturn
            | TypeStructureKind::T_arraykey
            | TypeStructureKind::T_mixed
            | TypeStructureKind::T_nonnull
            | TypeStructureKind::T_null
            | TypeStructureKind::T_nothing
            | TypeStructureKind::T_dynamic
            // resolved classnames
            | TypeStructureKind::T_darray
            | TypeStructureKind::T_varray
            | TypeStructureKind::T_varray_or_darray
            | TypeStructureKind::T_vec
            | TypeStructureKind::T_dict
            | TypeStructureKind::T_keyset
            | TypeStructureKind::T_vec_or_dict
            | TypeStructureKind::T_any_array
    )
}

fn shape_field_name(sf: &ShapeFieldName) -> (String, bool) {
    use oxidized::ast_defs::Id;
    match sf {
        ShapeFieldName::SFlitInt((_, s)) => (s.to_string(), false),
        ShapeFieldName::SFlitStr((_, s)) => {
            (
                // FIXME: This is not safe--string literals are binary strings.
                // There's no guarantee that they're valid UTF-8.
                unsafe { String::from_utf8_unchecked(s.clone().into()) },
                false,
            )
        }
        ShapeFieldName::SFclassConst(Id(_, cname), (_, s)) => {
            let id = hhbc::ClassName::from_ast_name_and_mangle(cname);
            (format!("{}::{}", id.as_str(), s), true)
        }
    }
}

fn shape_field_to_entry(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    type_refinement_in_hint: TypeRefinementInHint,
    sfi: &ShapeFieldInfo,
) -> Result<DictEntry> {
    let (name, is_class_const) = shape_field_name(&sfi.name);
    let mut r = vec![];
    if is_class_const {
        r.push(encode_entry("is_cls_cns", TypedValue::Bool(true)));
    }
    if sfi.optional {
        r.push(encode_entry("optional_shape_field", TypedValue::Bool(true)));
    };
    r.push(encode_entry(
        "value",
        hint_to_type_constant(opts, tparams, targ_map, &sfi.hint, type_refinement_in_hint)?,
    ));
    Ok(encode_entry(&name, TypedValue::dict(r)))
}

fn shape_info_to_typed_value(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    type_refinement_in_hint: TypeRefinementInHint,
    si: &NastShapeInfo,
) -> Result<TypedValue> {
    let info = si
        .field_map
        .iter()
        .map(|sfi| shape_field_to_entry(opts, tparams, targ_map, type_refinement_in_hint, sfi))
        .collect::<Result<Vec<_>>>()?;
    Ok(TypedValue::dict(info))
}

fn shape_allows_unknown_fields(si: &NastShapeInfo) -> Option<DictEntry> {
    if si.allows_unknown_fields {
        Some(encode_entry(
            "allows_unknown_fields",
            TypedValue::Bool(true),
        ))
    } else {
        None
    }
}

fn type_constant_access_list(ids: &[aast::Sid]) -> TypedValue {
    let ids: Vec<_> = ids
        .iter()
        .map(|ast_defs::Id(_, s)| TypedValue::intern_string(s))
        .collect();
    TypedValue::vec(ids)
}

fn resolve_classname(tparams: &[&str], mut s: String) -> (Option<DictEntry>, String) {
    let kind = ts_kind(tparams, &s);
    let name_key = if kind != TypeStructureKind::T_typevar {
        s = hhbc::ClassName::from_ast_name_and_mangle(s.as_str()).into_string();
        "classname"
    } else {
        "name"
    };
    let entry = if is_prim_or_resolved_classname(ts_kind(tparams, &s)) {
        None
    } else {
        Some(encode_entry(name_key, TypedValue::intern_string(&s)))
    };
    (entry, s)
}

fn get_generic_types(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    type_refinement_in_hint: TypeRefinementInHint,
    hints: &[Hint],
) -> Result<Vec<DictEntry>> {
    Ok(if hints.is_empty() {
        vec![]
    } else {
        vec![encode_entry(
            "generic_types",
            hints_to_type_constant(opts, tparams, targ_map, type_refinement_in_hint, hints)?,
        )]
    })
}

fn get_union_types(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    type_refinement_in_hint: TypeRefinementInHint,
    hints: &[Hint],
) -> Result<Vec<DictEntry>> {
    Ok(vec![encode_entry(
        "union_types",
        hints_to_type_constant(opts, tparams, targ_map, type_refinement_in_hint, hints)?,
    )])
}

fn encode_entry(key: &str, value: TypedValue) -> DictEntry {
    DictEntry {
        key: TypedValue::intern_string(key),
        value,
    }
}

fn encode_kind(kind: TypeStructureKind) -> DictEntry {
    encode_entry("kind", TypedValue::Int(kind.repr as i64))
}

fn encode_root_name(s: &str) -> DictEntry {
    let s = if s == "this" {
        string_utils::prefix_namespace("HH", s)
    } else {
        hhbc::ClassName::from_ast_name_and_mangle(s).into_string()
    };
    encode_entry("root_name", TypedValue::intern_string(s))
}

fn get_typevars(tparams: &[&str]) -> Vec<DictEntry> {
    if tparams.is_empty() {
        vec![]
    } else {
        vec![encode_entry(
            "typevars",
            TypedValue::intern_string(tparams.join(",")),
        )]
    }
}

fn hint_to_type_constant_list(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    type_refinement_in_hint: TypeRefinementInHint,
    Hint(_, hint): &Hint,
) -> Result<Vec<DictEntry>> {
    Ok(match hint.as_ref() {
        Hint_::Happly(s, hints) => {
            let ast_defs::Id(_, name) = s;
            if hints.is_empty() {
                if let Some(id) = targ_map.get(name.as_str()) {
                    return Ok(vec![
                        encode_kind(TypeStructureKind::T_reifiedtype),
                        encode_entry("id", TypedValue::Int(*id)),
                    ]);
                }
            }
            let hints = match &hints[..] {
                [h] if name == typehints::POISON_MARKER || name == typehints::HH_FUNCTIONREF => {
                    return hint_to_type_constant_list(
                        opts,
                        tparams,
                        targ_map,
                        type_refinement_in_hint,
                        h,
                    );
                }
                _ => hints,
            };
            let (classname, s_res) = resolve_classname(tparams, name.to_owned());
            let mut r = vec![];
            if s_res.eq_ignore_ascii_case("tuple") || s_res.eq_ignore_ascii_case("shape") {
                r.push(encode_kind(TypeStructureKind::T_unresolved));
            } else {
                r.push(encode_kind(ts_kind(tparams, s_res.as_str())));
            }
            if let Some(c) = classname {
                r.push(c);
            }
            if !(name.eq_ignore_ascii_case(classes::CLASS_NAME)
                || name.eq_ignore_ascii_case(classes::TYPE_NAME))
            {
                r.append(&mut get_generic_types(
                    opts,
                    tparams,
                    targ_map,
                    type_refinement_in_hint,
                    hints,
                )?);
            };
            r
        }
        Hint_::Hwildcard => {
            let (classname, _s_res) = resolve_classname(&[], "_".to_owned());
            let mut r = vec![];
            r.push(encode_kind(TypeStructureKind::T_typevar));
            if let Some(c) = classname {
                r.push(c);
            }
            r
        }
        Hint_::Hshape(si) => {
            let mut r = vec![];
            if let Some(v) = shape_allows_unknown_fields(si) {
                r.push(v);
            }
            r.push(encode_kind(TypeStructureKind::T_shape));
            r.push(encode_entry(
                "fields",
                shape_info_to_typed_value(opts, tparams, targ_map, type_refinement_in_hint, si)?,
            ));
            r
        }
        Hint_::Haccess(Hint(_, h), ids) => match h.as_happly() {
            Some((root_id, hs)) if hs.is_empty() => {
                vec![
                    encode_kind(TypeStructureKind::T_typeaccess),
                    encode_root_name(&root_id.1),
                    encode_entry("access_list", type_constant_access_list(ids)),
                ]
            }
            _ => {
                return Err(Error::unrecoverable(
                    "Structure not translated according to ast_to_nast",
                ));
            }
        },
        Hint_::Hfun(hf) => {
            let mut r = vec![];
            r.push(encode_kind(TypeStructureKind::T_fun));
            let single_hint = |name: &str, h| {
                hint_to_type_constant(opts, tparams, targ_map, h, type_refinement_in_hint)
                    .map(|tc| vec![encode_entry(name, tc)])
            };
            let mut return_type = single_hint("return_type", &hf.return_ty)?;
            let mut variadic_type = hf
                .variadic_ty
                .as_ref()
                .map_or_else(|| Ok(vec![]), |h| single_hint("variadic_type", h))?;
            let mut param_types = vec![encode_entry(
                "param_types",
                hints_to_type_constant(
                    opts,
                    tparams,
                    targ_map,
                    type_refinement_in_hint,
                    &hf.param_tys,
                )?,
            )];
            param_types.append(&mut variadic_type);
            return_type.append(&mut param_types);
            r.append(&mut return_type);
            r
        }
        Hint_::Htuple(hints) => {
            let mut r = vec![];
            r.push(encode_kind(TypeStructureKind::T_tuple));
            r.push(encode_entry(
                "elem_types",
                hints_to_type_constant(opts, tparams, targ_map, type_refinement_in_hint, hints)?,
            ));
            r
        }
        Hint_::Hoption(h) => {
            let mut r = vec![];
            r.push(encode_entry("nullable", TypedValue::Bool(true)));
            r.append(&mut hint_to_type_constant_list(
                opts,
                tparams,
                targ_map,
                type_refinement_in_hint,
                h,
            )?);
            r
        }
        Hint_::Hsoft(h) => {
            let mut r = vec![];
            r.push(encode_entry("soft", TypedValue::Bool(true)));
            r.append(&mut hint_to_type_constant_list(
                opts,
                tparams,
                targ_map,
                type_refinement_in_hint,
                h,
            )?);
            r
        }
        Hint_::Hlike(h) => {
            let mut r = vec![];
            r.append(&mut hint_to_type_constant_list(
                opts,
                tparams,
                targ_map,
                type_refinement_in_hint,
                h,
            )?);
            r
        }
        // TODO(coeffects) actually handle emission of context constants
        Hint_::Hintersection(_) => {
            let mut r = vec![];
            r.push(encode_kind(TypeStructureKind::T_mixed));
            r
        }
        Hint_::Hunion(hints) => {
            let mut r = vec![];
            r.push(encode_kind(TypeStructureKind::T_union));
            r.append(&mut get_union_types(
                opts,
                tparams,
                targ_map,
                type_refinement_in_hint,
                hints,
            )?);
            r
        }
        Hint_::Hrefinement(h, _) => {
            match type_refinement_in_hint {
                TypeRefinementInHint::Disallowed => {
                    let aast::Hint(pos, _) = h;
                    return Err(Error::fatal_parse(pos, "Refinement in type structure"));
                }
                TypeRefinementInHint::Allowed => {
                    // check recursively (e.g.: Class<T1, T2> with { ... })
                    hint_to_type_constant_list(
                        opts,
                        tparams,
                        targ_map,
                        TypeRefinementInHint::Allowed,
                        h,
                    )?
                }
            }
        }
        _ => {
            return Err(Error::unrecoverable(
                "Hints not available on the original AST",
            ));
        }
    })
}

pub(crate) fn typedef_to_type_structure(
    opts: &Options,
    tparams: &[&str],
    typedef: &ast::Typedef,
) -> Result<TypedValue> {
    let is_case_type = typedef.vis.is_case_type();

    // For a case type we always want to ensure that it's wrapped in a Hunion.
    let tmp;
    let kind = match (is_case_type, &typedef.kind) {
        (false, kind) => kind,
        (true, kind @ Hint(_, box Hint_::Hunion(_))) => kind,
        (true, hint @ Hint(pos, _)) => {
            tmp = Hint(pos.clone(), Box::new(Hint_::Hunion(vec![hint.clone()])));
            &tmp
        }
    };

    let mut tconsts = hint_to_type_constant_list(
        opts,
        tparams,
        &BTreeMap::new(),
        TypeRefinementInHint::Disallowed, // Note: only called by `emit_typedef
        kind,
    )?;
    tconsts.append(&mut get_typevars(tparams));
    if typedef.vis.is_opaque() || typedef.vis.is_opaque_module() {
        tconsts.push(encode_entry("opaque", TypedValue::Bool(true)));
    };
    let mangled_name = hhbc_string_utils::mangle(typedef.name.1.clone());
    let name = hhbc_string_utils::strip_global_ns(&mangled_name);
    let key = if is_case_type { "case_type" } else { "alias" };
    tconsts.push(encode_entry(key, TypedValue::intern_string(name)));
    Ok(TypedValue::dict(tconsts))
}

pub(crate) fn hint_to_type_constant(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    hint: &Hint,
    type_refinement_in_hint: TypeRefinementInHint,
) -> Result<TypedValue> {
    let tconsts =
        hint_to_type_constant_list(opts, tparams, targ_map, type_refinement_in_hint, hint)?;
    Ok(TypedValue::dict(tconsts))
}

fn hints_to_type_constant(
    opts: &Options,
    tparams: &[&str],
    targ_map: &BTreeMap<&str, i64>,
    type_refinement_in_hint: TypeRefinementInHint,
    hints: &[Hint],
) -> Result<TypedValue> {
    Ok(TypedValue::vec(
        hints
            .iter()
            .map(|h| hint_to_type_constant(opts, tparams, targ_map, h, type_refinement_in_hint))
            .collect::<Result<Vec<_>>>()?,
    ))
}
