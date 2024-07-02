// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use ir::instr::Hhbc;
use ir::ClassGetCMode;
use ir::Func;
use ir::FuncBuilder;
use ir::Immediate;
use ir::Instr;
use ir::IrRepr;
use ir::LocId;
use ir::LocalId;
use ir::MemberOpBuilder;
use ir::MethodFlags;
use ir::MethodName;
use ir::ReadonlyOp;
use ir::SpecialClsRef;
use ir::SrcLoc;
use ir::TypedValue;
use log::trace;

use crate::func::FuncInfo;
use crate::func::MethodInfo;
use crate::lower::class::THIS_AS_PROPERTY;

pub(crate) fn lower_func(mut func: Func, func_info: &mut FuncInfo<'_>) -> Func {
    trace!(
        "{} Before Lower: {}",
        func_info.name_id(),
        ir::print::DisplayFunc::new(&func, true,)
    );

    // In a closure we implicitly load all the properties as locals - so start
    // with that as a prelude to all entrypoints.
    match *func_info {
        FuncInfo::Method(ref mut method_info) => {
            if method_info.flags.contains(MethodFlags::IS_CLOSURE_BODY) {
                load_closure_vars(&mut func, method_info);
            }
        }
        FuncInfo::Function(_) => {}
    }

    // If the function is reified then make the implied 0ReifiedGenerics
    // parameter explicit.
    if func.is_reified() {
        add_reified_parameter(&mut func);
    }

    if func_info.declared_in_trait() {
        add_self_trait_parameter(&mut func);
    }

    // Start by 'unasync'ing the Func.
    ir::passes::unasync(&mut func);
    trace!(
        "After unasync: {}",
        ir::print::DisplayFunc::new(&func, true,)
    );

    let mut builder = FuncBuilder::with_func(func);

    match func_info {
        FuncInfo::Method(mi) if mi.name.is_86pinit() => {
            rewrite_86pinit(&mut builder, mi);
        }
        FuncInfo::Method(mi) if mi.name.is_86sinit() => {
            rewrite_86sinit(&mut builder, mi);
        }
        _ => {}
    }

    // Simplify various Instrs.
    super::instrs::lower_instrs(&mut builder, func_info);

    trace!(
        "After lower_instrs: {}",
        ir::print::DisplayFunc::new(&builder.func, true,)
    );

    // Write the complex constants out as a prelude to the function.
    super::constants::write_constants(&mut builder);

    let mut func = builder.finish();

    ir::passes::split_critical_edges(&mut func, true);

    ir::passes::clean::run(&mut func);

    trace!("After Lower: {}", ir::print::DisplayFunc::new(&func, true,));

    func
}

fn add_reified_parameter(func: &mut Func) {
    let param = ir::Param {
        name: ir::intern(hhbc_string_utils::reified::GENERICS_LOCAL_NAME),
        is_variadic: false,
        is_inout: false,
        is_readonly: false,
        user_attributes: Default::default(),
        type_info: ir::Maybe::Just(ir::TypeInfo {
            user_type: None.into(),
            type_constraint: ir::Constraint {
                name: Some(ir::intern(hhbc::BUILTIN_NAME_VEC)).into(),
                flags: ir::TypeConstraintFlags::NoFlags,
            },
        }),
    };
    func.repr.params.push((param, None));
}

fn add_self_trait_parameter(func: &mut Func) {
    // We insert a `self` parameter so infer's analysis can
    // do its job. We don't use `$` so we are sure we don't clash with
    // existing Hack user defined variables.
    func.repr.params.push((
        ir::Param {
            name: ir::string_id!("self"),
            is_variadic: false,
            is_inout: false,
            is_readonly: false,
            user_attributes: vec![].into(),
            type_info: ir::Maybe::Nothing,
        },
        None,
    ));
}

fn call_base_func(builder: &mut FuncBuilder, method_info: &MethodInfo<'_>, loc: LocId) {
    if method_info.class.base.is_just() {
        let clsref = SpecialClsRef::ParentCls;
        let method = method_info.name;
        builder.emit(Instr::method_call_special(clsref, method, &[], loc));
    }
}

fn rewrite_86pinit(builder: &mut FuncBuilder, method_info: &MethodInfo<'_>) {
    // In HHVM 86pinit is only used to initialize "complex" properties (and
    // doesn't exist if there aren't any). For textual we change that to use it
    // to initialize all properties and be guaranteed to exist.

    builder.start_block(IrRepr::ENTRY_BID);
    let saved = std::mem::take(&mut builder.cur_block_mut().iids);
    let loc = builder.add_loc(SrcLoc::from_span(&builder.func.span));

    call_base_func(builder, method_info, loc);

    // Init the properties.
    for prop in &method_info.class.properties {
        match prop {
            ir::Property {
                name,
                flags,
                initial_value: ir::Maybe::Just(initial_value),
                ..
            } if !flags.is_static() => {
                let vid = builder.emit_imm(initial_value.clone().into());
                MemberOpBuilder::base_h(loc).emit_set_m_pt(builder, *name, vid);
            }
            _ => {}
        }
    }

    builder.cur_block_mut().iids.extend(saved);
}

fn rewrite_86sinit(builder: &mut FuncBuilder, method_info: &MethodInfo<'_>) {
    // In HHVM 86sinit is only used to initialize "complex" static properties
    // (and doesn't exist if there aren't any). For textual we change that to
    // use it to initialize all properties and be guaranteed to exist.  We also
    // use it to initialize class constants.

    builder.start_block(IrRepr::ENTRY_BID);
    let saved = std::mem::take(&mut builder.cur_block_mut().iids);
    let loc = builder.add_loc(SrcLoc::from_span(&builder.func.span));

    call_base_func(builder, method_info, loc);

    let class = &method_info.class;

    let infer_const = ir::ClassName::intern(crate::lower::class::INFER_CONSTANT);

    // Now emit the static properties.
    let cls_name = builder.emit_imm(Immediate::String(class.name.as_bytes_id()));
    let cls = builder.emit(Instr::Hhbc(Hhbc::ClassGetC(
        cls_name,
        ClassGetCMode::Normal,
        loc,
    )));
    for prop in &class.properties {
        if !prop.flags.is_static() {
            continue;
        }
        let is_const = prop.attributes.iter().any(|attr| attr.name == infer_const);

        let vid = match prop {
            ir::Property {
                name,
                initial_value: ir::Maybe::Just(TypedValue::Uninit),
                ..
            } if is_const => {
                // This is a "complex" constant - we need to call 86cinit to get
                // the value.
                let clsref = SpecialClsRef::SelfCls;
                let method = MethodName::_86cinit();
                let name = builder.emit_imm(Immediate::String(name.as_bytes_id()));
                Some(builder.emit(Instr::method_call_special(clsref, method, &[name], loc)))
            }
            ir::Property {
                initial_value: ir::Maybe::Nothing,
                ..
            } if is_const => {
                // This is an abstract constant - its value will be overwritten
                // in a subclass's init - so just skip it.
                continue;
            }
            ir::Property {
                initial_value: ir::Maybe::Just(initial_value),
                ..
            } => {
                // Either a normal property or non-complex constant.
                Some(builder.emit_imm(initial_value.clone().into()))
            }
            _ => None,
        };

        if let Some(vid) = vid {
            let prop_name = builder.emit_imm(Immediate::String(prop.name.as_bytes_id()));
            builder.emit(Instr::Hhbc(Hhbc::SetS(
                [prop_name, cls, vid],
                ReadonlyOp::Any,
                loc,
            )));
        }
    }

    builder.cur_block_mut().iids.extend(saved);
}

fn load_closure_vars(func: &mut Func, method_info: &MethodInfo<'_>) {
    let mut instrs = Vec::new();
    let loc = ir::LocId::from_usize(0);

    // Some magic: We sort the properties such that we always put 'this' at the
    // end. That way when we overwrite the '$this' local we're doing so as the
    // last access to the closure class '$this'.
    let sort_cmp = |a: &ir::Property, b: &ir::Property| -> std::cmp::Ordering {
        if a.name.as_str() == THIS_AS_PROPERTY {
            std::cmp::Ordering::Greater
        } else if b.name.as_str() == THIS_AS_PROPERTY {
            std::cmp::Ordering::Less
        } else {
            a.name.cmp(&b.name)
        }
    };

    let mut properties = method_info.class.properties.to_vec();
    properties.sort_unstable_by(sort_cmp);

    for prop in &properties {
        // Property names are the variable names without the '$'.
        let var = format!("${}", prop.name);
        let lid = LocalId::Named(ir::intern(var));
        let iid = func.repr.alloc_instr(Instr::MemberOp(
            MemberOpBuilder::base_h(loc).query_pt(prop.name),
        ));
        instrs.push(iid);
        instrs.push(
            func.repr
                .alloc_instr(Instr::Hhbc(Hhbc::SetL(iid.into(), lid, loc))),
        );
    }

    func.repr
        .block_mut(IrRepr::ENTRY_BID)
        .iids
        .splice(0..0, instrs);
}
