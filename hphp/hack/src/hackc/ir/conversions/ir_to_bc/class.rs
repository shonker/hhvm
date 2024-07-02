// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use crate::convert::UnitBuilder;

pub(crate) fn convert_class(unit: &mut UnitBuilder, class: ir::Class) {
    let ir::Class {
        attributes,
        base,
        constants,
        ctx_constants,
        doc_comment,
        enum_type,
        enum_includes,
        flags,
        implements,
        methods,
        name,
        properties,
        requirements,
        span,
        type_constants,
        upper_bounds,
        uses,
    } = class;

    let methods = Vec::from_iter(
        methods
            .into_iter()
            .map(|method| crate::func::convert_method(method, &mut unit.adata_cache)),
    );

    let class = hhbc::Class {
        attributes: attributes.into(),
        base: base.into(),
        constants: constants.into(),
        ctx_constants: ctx_constants.into(),
        doc_comment: doc_comment.map(|c| c.into()).into(),
        enum_includes: enum_includes.into(),
        enum_type: enum_type.into(),
        flags,
        implements: implements.into(),
        methods: methods.into(),
        name,
        properties: properties.into(),
        requirements: requirements.into(),
        span,
        type_constants: type_constants.into(),
        upper_bounds: upper_bounds.into(),
        uses: uses.into(),
    };
    unit.classes.push(class);
}
