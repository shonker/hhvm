// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use bstr::BStr;
use bstr::ByteSlice;
use naming_special_names_rust::members;
use serde::Serialize;

use crate::StringId;

/// Generates `$type` as a newtype `StringId`, which is a globally interned
/// unique reference to a &'static str.
macro_rules! impl_intern_id {
    ($type: ident) => {
        impl $type {
            pub const fn new(s: StringId) -> Self {
                Self(s)
            }

            pub fn empty() -> Self {
                Self(StringId::EMPTY)
            }

            pub fn is_empty(self) -> bool {
                self.0 == StringId::EMPTY
            }

            pub fn into_string(self) -> std::string::String {
                self.0.as_str().into()
            }

            pub fn as_str(&self) -> &'static str {
                self.0.as_str()
            }

            pub fn as_bstr(&self) -> &'static BStr {
                self.as_bytes().as_bstr()
            }

            pub fn as_bytes(&self) -> &'static [u8] {
                self.0.as_str().as_bytes()
            }

            pub fn as_string_id(&self) -> crate::StringId {
                self.0
            }

            pub fn as_bytes_id(&self) -> crate::BytesId {
                self.0.as_bytes()
            }

            pub fn intern(s: &str) -> Self {
                Self(crate::intern(s))
            }

            pub fn from_utf8(s: &[u8]) -> Result<Self, std::str::Utf8Error> {
                Ok(Self::intern(std::str::from_utf8(s)?))
            }

            pub fn from_bytes(id: crate::BytesId) -> Result<Self, std::str::Utf8Error> {
                Ok(Self(StringId::from_bytes(id)?))
            }
        }

        impl write_bytes::DisplayBytes for $type {
            fn fmt(&self, f: &mut write_bytes::BytesFormatter<'_>) -> std::io::Result<()> {
                self.as_bytes().fmt(f)
            }
        }

        impl std::fmt::Debug for $type {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                write!(f, "{}({})", module_path!(), self.as_str())
            }
        }

        impl std::fmt::Display for $type {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                self.0.fmt(f)
            }
        }
    };
}

macro_rules! impl_intern_add_suffix {
    ($type: ident) => {
        impl $type {
            fn from_str_with_suffix(prefix: &str, suffix: &str) -> $type {
                let mut r = String::with_capacity(prefix.len() + suffix.len());
                r.push_str(prefix);
                r.push_str(suffix);
                $type::intern(&r)
            }

            pub fn add_suffix(id: &Self, suffix: &str) -> Self {
                $type::from_str_with_suffix(id.0.as_str(), suffix)
            }
        }
    };
}

/// Conventionally this is "A_" followed by an integer
#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct AdataId {
    id: u32,
}

impl AdataId {
    pub const INVALID: Self = Self { id: u32::MAX };

    pub fn new(id: usize) -> Self {
        Self {
            id: id.try_into().unwrap(),
        }
    }

    pub fn parse(s: &str) -> Result<Self, std::num::ParseIntError> {
        use std::str::FromStr;
        Ok(Self {
            id: u32::from_str(s.strip_prefix("A_").unwrap_or(""))?,
        })
    }

    pub fn id(&self) -> u32 {
        self.id
    }

    pub fn index(&self) -> usize {
        self.id as usize
    }
}

impl std::fmt::Display for AdataId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "A_{}", self.id)
    }
}

#[derive(Copy, Clone, PartialEq, Eq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct ClassName(StringId);

impl_intern_id!(ClassName);

impl ClassName {
    pub fn from_ast_name_and_mangle(s: impl std::convert::Into<std::string::String>) -> Self {
        Self(Self::mangle(s))
    }

    pub fn mangle(s: impl std::convert::Into<std::string::String>) -> StringId {
        intern::string::intern(hhbc_string_utils::strip_global_ns(
            &hhbc_string_utils::mangle(s.into()),
        ))
    }

    pub fn unmangled(&self) -> String {
        hhbc_string_utils::unmangle(self.as_str().into())
    }
}

#[derive(Copy, Clone, PartialEq, Eq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct ModuleName(StringId);

impl_intern_id!(ModuleName);

#[derive(Copy, Clone, PartialEq, Eq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct PropName(StringId);

impl_intern_id!(PropName);

impl PropName {
    pub fn from_ast_name(s: &str) -> Self {
        Self::intern(hhbc_string_utils::strip_global_ns(s))
    }
}

#[derive(Copy, Clone, PartialEq, Eq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct MethodName(StringId);

impl_intern_id!(MethodName);
impl_intern_add_suffix!(MethodName);

impl MethodName {
    pub fn from_ast_name(s: &str) -> Self {
        MethodName::intern(hhbc_string_utils::strip_global_ns(s))
    }

    pub fn from_ast_name_and_suffix(s: &str, suffix: &str) -> Self {
        MethodName::from_str_with_suffix(hhbc_string_utils::strip_global_ns(s), suffix)
    }

    pub fn _86cinit() -> Self {
        Self::intern(_86CINIT)
    }

    pub fn _86pinit() -> Self {
        Self::intern(_86PINIT)
    }

    pub fn _86sinit() -> Self {
        Self::intern(_86SINIT)
    }

    pub fn constructor() -> Self {
        Self::intern(members::__CONSTRUCT)
    }

    pub fn factory() -> Self {
        Self::intern(__FACTORY)
    }

    pub fn is_86cinit(&self) -> bool {
        self.as_str() == _86CINIT
    }

    pub fn is_86pinit(&self) -> bool {
        self.as_str() == _86PINIT
    }

    pub fn is_86sinit(&self) -> bool {
        self.as_str() == _86SINIT
    }

    pub fn is_constructor(&self) -> bool {
        self.as_str() == members::__CONSTRUCT
    }
}

const __FACTORY: &str = "__factory";
const _86CINIT: &str = "86cinit";
const _86PINIT: &str = "86pinit";
const _86SINIT: &str = "86sinit";

#[derive(Copy, Clone, Eq, PartialEq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct FunctionName(StringId);

impl_intern_id!(FunctionName);
impl_intern_add_suffix!(FunctionName);

impl FunctionName {
    pub fn from_ast_name(s: &str) -> Self {
        Self::intern(hhbc_string_utils::strip_global_ns(s))
    }
}

#[derive(Copy, Clone, PartialEq, Eq, Hash, Ord, PartialOrd, Serialize)]
#[repr(C)]
pub struct ConstName(StringId);

impl_intern_id!(ConstName);

impl ConstName {
    pub fn from_ast_name(s: &str) -> ConstName {
        ConstName::intern(hhbc_string_utils::strip_global_ns(s))
    }
}

#[cfg(test)]
mod tests {
    use std::collections::BTreeSet;

    use super::*;

    #[test]
    fn test_from_unsafe_as_str() {
        assert_eq!("Foo", ClassName::intern("Foo").as_str());
    }

    #[test]
    fn test_add_suffix() {
        let id = FunctionName::intern("Some");
        let id = FunctionName::add_suffix(&id, "Func");
        assert_eq!("SomeFunc", id.as_str());
    }

    #[test]
    fn test_from_ast_name() {
        let id = MethodName::from_ast_name("meth");
        assert_eq!("meth", id.as_str());
    }

    #[test]
    fn test_eq_function_name() {
        let id1 = FunctionName::from_ast_name("foo2$memoize_impl");
        let id2 = FunctionName::from_ast_name("Foo2$memoize_impl");
        assert_ne!(id1, id2);
    }

    #[test]
    fn test_ord_function_name() {
        let mut ids = BTreeSet::new();
        ids.insert(FunctionName::from_ast_name("foo"));
        ids.insert(FunctionName::from_ast_name("Foo"));
        ids.insert(FunctionName::from_ast_name("foo2"));
        ids.insert(FunctionName::from_ast_name("Bar"));
        ids.insert(FunctionName::from_ast_name("bar"));
        let expected = ["Bar", "Foo", "bar", "foo", "foo2"];
        let ids: Vec<&str> = ids.into_iter().map(|id| id.as_str()).collect();
        assert_eq!(expected, ids.as_slice());
    }
}
