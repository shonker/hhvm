// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use crate::{gen::global_options::GlobalOptions, i_set, infer_missing, s_map, s_set};

impl Default for GlobalOptions {
    fn default() -> Self {
        Self {
            tco_safe_array: false,
            tco_safe_vector_array: false,
            tco_experimental_features: s_set::SSet::new(),
            tco_migration_flags: s_set::SSet::new(),
            tco_dynamic_view: false,
            tco_defer_class_declaration_threshold: None,
            tco_prefetch_deferred_files: false,
            tco_remote_type_check_threshold: None,
            tco_remote_type_check: false,
            tco_remote_worker_key: None,
            tco_remote_check_id: None,
            tco_num_remote_workers: 0,
            so_remote_version_specifier: None,
            so_remote_worker_eden_checkout_threshold: 0,
            so_naming_sqlite_path: None,
            tco_disallow_array_as_tuple: false,
            po_auto_namespace_map: vec![],
            po_codegen: false,
            po_deregister_php_stdlib: false,
            po_disallow_execution_operator: false,
            po_disallow_toplevel_requires: false,
            po_disable_nontoplevel_declarations: false,
            po_disable_static_closures: false,
            po_disable_halt_compiler: false,
            po_allow_goto: false,
            tco_log_inference_constraints: false,
            tco_disallow_ambiguous_lambda: false,
            tco_disallow_array_typehint: false,
            tco_disallow_array_literal: false,
            tco_language_feature_logging: false,
            tco_unsafe_rx: false,
            tco_disallow_unset_on_varray: false,
            tco_disallow_scrutinee_case_value_type_mismatch: false,
            tco_new_inference_lambda: false,
            tco_timeout: 0,
            tco_disallow_invalid_arraykey: false,
            tco_disallow_byref_dynamic_calls: false,
            tco_disallow_byref_calls: false,
            ignored_fixme_codes: i_set::ISet::new(),
            ignored_fixme_regex: None,
            log_levels: s_map::SMap::new(),
            po_disable_lval_as_an_expression: false,
            tco_typecheck_xhp_cvars: false,
            tco_ignore_collection_expr_type_arguments: false,
            tco_shallow_class_decl: false,
            po_rust_parser_errors: false,
            profile_type_check_duration_threshold: 0.0,
            tco_like_types: false,
            tco_pessimize_types: false,
            tco_simple_pessimize: 0.0,
            tco_coercion_from_dynamic: false,
            tco_complex_coercion: false,
            tco_disable_partially_abstract_typeconsts: false,
            error_codes_treated_strictly: i_set::ISet::new(),
            tco_check_xhp_attribute: false,
            tco_disallow_unresolved_type_variables: false,
            tco_disallow_invalid_arraykey_constraint: false,
            po_enable_constant_visibility_modifiers: false,
            po_enable_class_level_where_clauses: false,
            po_disable_legacy_soft_typehints: false,
            tco_use_lru_workers: false,
            po_disallowed_decl_fixmes: i_set::ISet::new(),
            po_allow_new_attribute_syntax: false,
            tco_infer_missing: infer_missing::InferMissing::Deactivated,
            tco_const_static_props: false,
            po_disable_legacy_attribute_syntax: false,
            tco_const_attribute: false,
            po_const_default_func_args: false,
            po_disallow_silence: false,
            po_abstract_static_props: false,
            po_disable_unset_class_const: false,
            po_parser_errors_only: false,
            tco_check_attribute_locations: false,
        }
    }
}
