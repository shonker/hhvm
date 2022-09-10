// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use crate::Attribute;
use crate::ClassId;
use crate::SrcLoc;

pub struct Module<'a> {
    pub attributes: Vec<Attribute<'a>>,
    pub name: ClassId,
    pub span: SrcLoc,
}
