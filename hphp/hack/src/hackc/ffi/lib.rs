// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

//! There are basically two kinds of types can't be made `repr(C)`:
//!   * builtin types making no guaratees about binary layout (e.g.
//!     wide pointers, tuples)
//!   * type with definitions that are beyond our reach to annotate
//!     with `repr(C)` (e.g. standard or vendored types).
//!
//! The types in this crate are C friendly substitutions for a
//! handful of these types.

#![cfg_attr(UNSTABLE_DEBUG_SLICE, feature(min_specialization))]
pub mod maybe;
pub mod vector;

pub use triomphe::OffsetArc;

pub use crate::maybe::Maybe;
pub use crate::maybe::Maybe::*;
pub use crate::vector::Vector;
