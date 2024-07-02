(*
 * Copyright (c) 2015, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

[@@@warning "-33"]

open Hh_prelude

[@@@warning "+33"]

(* NOTE: this file is in the middle of a large refactoring.
   Please try to avoid changes other than adding a field to struct `t`,
   updating `default` value and `make` functions
   Encapsulation and helpers better fit in the respective modules: e.g.,
   TypecheckerOptions for tco_* fields
   ParserOptions for po_*fields
   etc.
*)

type saved_state_loading = {
  saved_state_manifold_api_key: string option;
  log_saved_state_age_and_distance: bool;
  use_manifold_cython_client: bool;
  zstd_decompress_by_file: bool;
  use_compressed_dep_graph: bool;
}
[@@deriving show, eq]

let default_saved_state_loading =
  {
    saved_state_manifold_api_key = None;
    log_saved_state_age_and_distance = false;
    use_manifold_cython_client = false;
    zstd_decompress_by_file = true;
    use_compressed_dep_graph = true;
  }

type saved_state = {
  loading: saved_state_loading;
  rollouts: Saved_state_rollouts.t;
  project_metadata_w_flags: bool;
}
[@@deriving show, eq]

type 'a all_or_some =
  | All
  | ASome of 'a list
[@@deriving eq, show]

let default_saved_state =
  {
    loading = default_saved_state_loading;
    rollouts = Saved_state_rollouts.default;
    project_metadata_w_flags = true;
  }

let with_saved_state_manifold_api_key saved_state_manifold_api_key ss =
  { ss with loading = { ss.loading with saved_state_manifold_api_key } }

let with_use_manifold_cython_client use_manifold_cython_client ss =
  { ss with loading = { ss.loading with use_manifold_cython_client } }

let with_log_saved_state_age_and_distance log_saved_state_age_and_distance ss =
  { ss with loading = { ss.loading with log_saved_state_age_and_distance } }

let with_zstd_decompress_by_file zstd_decompress_by_file ss =
  { ss with loading = { ss.loading with zstd_decompress_by_file } }

type extended_reasons_config =
  | Extended of int
  | Debug
  | Yolo
[@@deriving eq, show]

(** Naming conventions for fields in this struct:
  - tco_<feature/flag/setting> - type checker option
  - po_<feature/flag/setting> - parser option
  - so_<feature/flag/setting> - server option
*)
type t = {
  po: ParserOptions.t;
  tco_saved_state: saved_state;
  tco_experimental_features: SSet.t;
  tco_migration_flags: SSet.t;
  tco_num_local_workers: int option;
  tco_defer_class_declaration_threshold: int option;
  tco_locl_cache_capacity: int;
  tco_locl_cache_node_threshold: int;
  so_naming_sqlite_path: string option;
  po_disallow_toplevel_requires: bool;
  tco_log_large_fanouts_threshold: int option;
  tco_log_inference_constraints: bool;
  tco_language_feature_logging: bool;
  tco_timeout: int;
  tco_disallow_invalid_arraykey: bool;
  code_agnostic_fixme: bool;
  allowed_fixme_codes_strict: ISet.t;
  log_levels: int SMap.t;
  class_pointer_levels: int SMap.t;
  tco_remote_old_decls_no_limit: bool;
  tco_use_old_decls_from_cas: bool;
  tco_fetch_remote_old_decls: bool;
  tco_populate_member_heaps: bool;
  tco_skip_hierarchy_checks: bool;
  tco_skip_tast_checks: bool;
  tco_like_type_hints: bool;
  tco_coeffects: bool;
  tco_coeffects_local: bool;
  tco_strict_contexts: bool;
  tco_like_casts: bool;
  tco_check_xhp_attribute: bool;
  tco_check_redundant_generics: bool;
  tco_disallow_unresolved_type_variables: bool;
  tco_custom_error_config: Custom_error_config.t;
  tco_const_attribute: bool;
  tco_check_attribute_locations: bool;
  glean_reponame: string;
  symbol_write_index_inherited_members: bool;
  symbol_write_ownership: bool;
  symbol_write_root_path: string;
  symbol_write_hhi_path: string;
  symbol_write_ignore_paths: string list;
  symbol_write_index_paths: string list;
  symbol_write_index_paths_file: string option;
  symbol_write_index_paths_file_output: string option;
  symbol_write_include_hhi: bool;
  symbol_write_sym_hash_in: string option;
  symbol_write_exclude_out: string option;
  symbol_write_referenced_out: string option;
  symbol_write_reindexed_out: string option;
  symbol_write_sym_hash_out: bool;
  tco_error_php_lambdas: bool;
  tco_disallow_discarded_nullable_awaitables: bool;
  tco_higher_kinded_types: bool;
  tco_report_pos_from_reason: bool;
  tco_typecheck_sample_rate: float;
  tco_enable_sound_dynamic: bool;
  tco_pessimise_builtins: bool;
  tco_enable_no_auto_dynamic: bool;
  tco_skip_check_under_dynamic: bool;
  tco_global_access_check_enabled: bool;
  tco_enable_strict_string_concat_interp: bool;
  tco_ignore_unsafe_cast: bool;
  tco_enable_expression_trees: bool;
  tco_enable_function_references: bool;
  tco_allowed_expression_tree_visitors: string list;
  tco_typeconst_concrete_concrete_error: bool;
  tco_enable_strict_const_semantics: int;
  tco_strict_wellformedness: int;
  tco_meth_caller_only_public_visibility: bool;
  tco_require_extends_implements_ancestors: bool;
  tco_strict_value_equality: bool;
  tco_enforce_sealed_subclasses: bool;
  tco_implicit_inherit_sdt: bool;
  tco_explicit_consistent_constructors: int;
  tco_require_types_class_consts: int;
  tco_type_printer_fuel: int;
  tco_specify_manifold_api_key: bool;
  tco_profile_top_level_definitions: bool;
  tco_allow_all_files_for_module_declarations: bool;
  tco_allowed_files_for_module_declarations: string list;
  tco_record_fine_grained_dependencies: bool;
  tco_loop_iteration_upper_bound: int option;
  tco_use_type_alias_heap: bool;
  tco_populate_dead_unsafe_cast_heap: bool;
  tco_rust_elab: bool;
  dump_tast_hashes: bool;
  dump_tasts: string list;
  tco_autocomplete_mode: bool;
  tco_package_info: PackageInfo.t;
  tco_log_exhaustivity_check: bool;
  tco_sticky_quarantine: bool;
  tco_lsp_invalidation: bool;
  tco_autocomplete_sort_text: bool;
  tco_extended_reasons: extended_reasons_config option;
  hack_warnings: int all_or_some;
  tco_strict_switch: bool;
  tco_allowed_files_for_ignore_readonly: string list;
  tco_package_v2: bool;
  tco_package_v2_bypass_package_check_for_class_const: bool;
  preexisting_warnings: bool;
  re_no_cache: bool;
  hh_distc_should_disable_trace_store: bool;
  tco_enable_abstract_method_optional_parameters: bool;
}
[@@deriving eq, show]

let default =
  {
    po = ParserOptions.default;
    tco_saved_state = default_saved_state;
    tco_experimental_features = SSet.empty;
    tco_migration_flags = SSet.empty;
    tco_num_local_workers = None;
    tco_defer_class_declaration_threshold = None;
    tco_locl_cache_capacity = 30;
    tco_locl_cache_node_threshold = 10_000;
    so_naming_sqlite_path = None;
    po_disallow_toplevel_requires = false;
    tco_log_large_fanouts_threshold = None;
    tco_log_inference_constraints = false;
    tco_language_feature_logging = false;
    tco_timeout = 0;
    tco_disallow_invalid_arraykey = true;
    code_agnostic_fixme = false;
    allowed_fixme_codes_strict = ISet.empty;
    log_levels = SMap.empty;
    class_pointer_levels = SMap.empty;
    tco_remote_old_decls_no_limit = false;
    tco_use_old_decls_from_cas = false;
    tco_fetch_remote_old_decls = true;
    tco_populate_member_heaps = true;
    tco_skip_hierarchy_checks = false;
    tco_skip_tast_checks = false;
    tco_like_type_hints = false;
    tco_coeffects = true;
    tco_coeffects_local = true;
    tco_strict_contexts = true;
    tco_like_casts = false;
    tco_check_xhp_attribute = false;
    tco_check_redundant_generics = false;
    tco_disallow_unresolved_type_variables = false;
    tco_custom_error_config = Custom_error_config.empty;
    tco_const_attribute = false;
    tco_check_attribute_locations = true;
    glean_reponame = "www.hack.light";
    symbol_write_index_inherited_members = true;
    symbol_write_ownership = false;
    symbol_write_root_path = "www";
    symbol_write_hhi_path = "hhi";
    symbol_write_ignore_paths = [];
    symbol_write_index_paths = [];
    symbol_write_index_paths_file = None;
    symbol_write_index_paths_file_output = None;
    symbol_write_include_hhi = false;
    symbol_write_sym_hash_in = None;
    symbol_write_exclude_out = None;
    symbol_write_referenced_out = None;
    symbol_write_reindexed_out = None;
    symbol_write_sym_hash_out = false;
    tco_error_php_lambdas = false;
    tco_disallow_discarded_nullable_awaitables = false;
    tco_higher_kinded_types = false;
    tco_report_pos_from_reason = false;
    tco_typecheck_sample_rate = 1.0;
    tco_enable_sound_dynamic = false;
    tco_pessimise_builtins = false;
    tco_enable_no_auto_dynamic = false;
    tco_skip_check_under_dynamic = false;
    tco_global_access_check_enabled = false;
    tco_enable_strict_string_concat_interp = false;
    tco_ignore_unsafe_cast = false;
    tco_enable_expression_trees = false;
    tco_enable_function_references = false;
    tco_allowed_expression_tree_visitors = [];
    tco_typeconst_concrete_concrete_error = false;
    tco_enable_strict_const_semantics = 0;
    tco_strict_wellformedness = 0;
    tco_meth_caller_only_public_visibility = true;
    tco_require_extends_implements_ancestors = false;
    tco_strict_value_equality = false;
    tco_enforce_sealed_subclasses = false;
    tco_implicit_inherit_sdt = false;
    tco_explicit_consistent_constructors = 0;
    tco_require_types_class_consts = 0;
    tco_type_printer_fuel = 100;
    tco_specify_manifold_api_key = false;
    tco_profile_top_level_definitions = false;
    tco_allow_all_files_for_module_declarations = true;
    tco_allowed_files_for_module_declarations = [];
    tco_record_fine_grained_dependencies = false;
    tco_loop_iteration_upper_bound = None;
    tco_use_type_alias_heap = false;
    tco_populate_dead_unsafe_cast_heap = false;
    tco_rust_elab = false;
    dump_tast_hashes = false;
    dump_tasts = [];
    tco_autocomplete_mode = false;
    tco_package_info = PackageInfo.empty;
    tco_log_exhaustivity_check = false;
    tco_sticky_quarantine = false;
    tco_lsp_invalidation = false;
    tco_autocomplete_sort_text = false;
    tco_extended_reasons = None;
    hack_warnings = ASome [];
    tco_strict_switch = false;
    tco_allowed_files_for_ignore_readonly = [];
    tco_package_v2 = false;
    tco_package_v2_bypass_package_check_for_class_const = true;
    preexisting_warnings = false;
    re_no_cache = false;
    hh_distc_should_disable_trace_store = false;
    tco_enable_abstract_method_optional_parameters = false;
  }

let set
    ?po
    ?tco_saved_state
    ?po_disallow_toplevel_requires
    ?tco_log_large_fanouts_threshold
    ?tco_log_inference_constraints
    ?tco_experimental_features
    ?tco_migration_flags
    ?tco_num_local_workers
    ?tco_defer_class_declaration_threshold
    ?tco_locl_cache_capacity
    ?tco_locl_cache_node_threshold
    ?so_naming_sqlite_path
    ?tco_language_feature_logging
    ?tco_timeout
    ?tco_disallow_invalid_arraykey
    ?code_agnostic_fixme
    ?allowed_fixme_codes_strict
    ?log_levels
    ?class_pointer_levels
    ?tco_remote_old_decls_no_limit
    ?tco_use_old_decls_from_cas
    ?tco_fetch_remote_old_decls
    ?tco_populate_member_heaps
    ?tco_skip_hierarchy_checks
    ?tco_skip_tast_checks
    ?tco_like_type_hints
    ?tco_coeffects
    ?tco_coeffects_local
    ?tco_strict_contexts
    ?tco_like_casts
    ?tco_check_xhp_attribute
    ?tco_check_redundant_generics
    ?tco_disallow_unresolved_type_variables
    ?tco_custom_error_config
    ?tco_const_attribute
    ?tco_check_attribute_locations
    ?glean_reponame
    ?symbol_write_index_inherited_members
    ?symbol_write_ownership
    ?symbol_write_root_path
    ?symbol_write_hhi_path
    ?symbol_write_ignore_paths
    ?symbol_write_index_paths
    ?symbol_write_index_paths_file
    ?symbol_write_index_paths_file_output
    ?symbol_write_include_hhi
    ?symbol_write_sym_hash_in
    ?symbol_write_exclude_out
    ?symbol_write_referenced_out
    ?symbol_write_reindexed_out
    ?symbol_write_sym_hash_out
    ?tco_error_php_lambdas
    ?tco_disallow_discarded_nullable_awaitables
    ?tco_higher_kinded_types
    ?tco_report_pos_from_reason
    ?tco_typecheck_sample_rate
    ?tco_enable_sound_dynamic
    ?tco_pessimise_builtins
    ?tco_enable_no_auto_dynamic
    ?tco_skip_check_under_dynamic
    ?tco_global_access_check_enabled
    ?tco_enable_strict_string_concat_interp
    ?tco_ignore_unsafe_cast
    ?tco_enable_expression_trees
    ?tco_enable_function_references
    ?tco_allowed_expression_tree_visitors
    ?tco_typeconst_concrete_concrete_error
    ?tco_enable_strict_const_semantics
    ?tco_strict_wellformedness
    ?tco_meth_caller_only_public_visibility
    ?tco_require_extends_implements_ancestors
    ?tco_strict_value_equality
    ?tco_enforce_sealed_subclasses
    ?tco_implicit_inherit_sdt
    ?tco_explicit_consistent_constructors
    ?tco_require_types_class_consts
    ?tco_type_printer_fuel
    ?tco_specify_manifold_api_key
    ?tco_profile_top_level_definitions
    ?tco_allow_all_files_for_module_declarations
    ?tco_allowed_files_for_module_declarations
    ?tco_record_fine_grained_dependencies
    ?tco_loop_iteration_upper_bound
    ?tco_use_type_alias_heap
    ?tco_populate_dead_unsafe_cast_heap
    ?tco_rust_elab
    ?dump_tast_hashes
    ?dump_tasts
    ?tco_autocomplete_mode
    ?tco_package_info
    ?tco_log_exhaustivity_check
    ?tco_sticky_quarantine
    ?tco_lsp_invalidation
    ?tco_autocomplete_sort_text
    ?tco_extended_reasons
    ?hack_warnings
    ?tco_strict_switch
    ?tco_allowed_files_for_ignore_readonly
    ?tco_package_v2
    ?tco_package_v2_bypass_package_check_for_class_const
    ?preexisting_warnings
    ?re_no_cache
    ?hh_distc_should_disable_trace_store
    ?tco_enable_abstract_method_optional_parameters
    options =
  let setting setting option =
    match setting with
    | None -> option
    | Some value -> value
  in
  let setting_opt setting option =
    match setting with
    | None -> option
    | Some _ -> setting
  in
  {
    po = setting po options.po;
    tco_saved_state = setting tco_saved_state options.tco_saved_state;
    tco_experimental_features =
      setting tco_experimental_features options.tco_experimental_features;
    tco_migration_flags =
      setting tco_migration_flags options.tco_migration_flags;
    tco_num_local_workers =
      setting_opt tco_num_local_workers options.tco_num_local_workers;
    tco_defer_class_declaration_threshold =
      setting_opt
        tco_defer_class_declaration_threshold
        options.tco_defer_class_declaration_threshold;
    tco_locl_cache_capacity =
      setting tco_locl_cache_capacity options.tco_locl_cache_capacity;
    tco_locl_cache_node_threshold =
      setting
        tco_locl_cache_node_threshold
        options.tco_locl_cache_node_threshold;
    so_naming_sqlite_path =
      setting_opt so_naming_sqlite_path options.so_naming_sqlite_path;
    code_agnostic_fixme =
      setting code_agnostic_fixme options.code_agnostic_fixme;
    allowed_fixme_codes_strict =
      setting allowed_fixme_codes_strict options.allowed_fixme_codes_strict;
    po_disallow_toplevel_requires =
      setting
        po_disallow_toplevel_requires
        options.po_disallow_toplevel_requires;
    tco_log_large_fanouts_threshold =
      setting_opt
        tco_log_large_fanouts_threshold
        options.tco_log_large_fanouts_threshold;
    tco_log_inference_constraints =
      setting
        tco_log_inference_constraints
        options.tco_log_inference_constraints;
    tco_language_feature_logging =
      setting tco_language_feature_logging options.tco_language_feature_logging;
    tco_timeout = setting tco_timeout options.tco_timeout;
    tco_disallow_invalid_arraykey =
      setting
        tco_disallow_invalid_arraykey
        options.tco_disallow_invalid_arraykey;
    log_levels = setting log_levels options.log_levels;
    class_pointer_levels =
      setting class_pointer_levels options.class_pointer_levels;
    tco_remote_old_decls_no_limit =
      setting
        tco_remote_old_decls_no_limit
        options.tco_remote_old_decls_no_limit;
    tco_use_old_decls_from_cas =
      setting tco_use_old_decls_from_cas options.tco_use_old_decls_from_cas;
    tco_fetch_remote_old_decls =
      setting tco_fetch_remote_old_decls options.tco_fetch_remote_old_decls;
    tco_populate_member_heaps =
      setting tco_populate_member_heaps options.tco_populate_member_heaps;
    tco_skip_hierarchy_checks =
      setting tco_skip_hierarchy_checks options.tco_skip_hierarchy_checks;
    tco_skip_tast_checks =
      setting tco_skip_tast_checks options.tco_skip_tast_checks;
    tco_like_type_hints =
      setting tco_like_type_hints options.tco_like_type_hints;
    tco_coeffects = setting tco_coeffects options.tco_coeffects;
    tco_coeffects_local =
      setting tco_coeffects_local options.tco_coeffects_local;
    tco_strict_contexts =
      setting tco_strict_contexts options.tco_strict_contexts;
    tco_like_casts = setting tco_like_casts options.tco_like_casts;
    tco_check_xhp_attribute =
      setting tco_check_xhp_attribute options.tco_check_xhp_attribute;
    tco_check_redundant_generics =
      setting tco_check_redundant_generics options.tco_check_redundant_generics;
    tco_disallow_unresolved_type_variables =
      setting
        tco_disallow_unresolved_type_variables
        options.tco_disallow_unresolved_type_variables;
    tco_custom_error_config =
      setting tco_custom_error_config options.tco_custom_error_config;
    tco_const_attribute =
      setting tco_const_attribute options.tco_const_attribute;
    tco_check_attribute_locations =
      setting
        tco_check_attribute_locations
        options.tco_check_attribute_locations;
    glean_reponame = setting glean_reponame options.glean_reponame;
    symbol_write_index_inherited_members =
      setting
        symbol_write_index_inherited_members
        options.symbol_write_index_inherited_members;
    symbol_write_ownership =
      setting symbol_write_ownership options.symbol_write_ownership;
    symbol_write_root_path =
      setting symbol_write_root_path options.symbol_write_root_path;
    symbol_write_hhi_path =
      setting symbol_write_hhi_path options.symbol_write_hhi_path;
    symbol_write_ignore_paths =
      setting symbol_write_ignore_paths options.symbol_write_ignore_paths;
    symbol_write_index_paths =
      setting symbol_write_index_paths options.symbol_write_index_paths;
    symbol_write_index_paths_file =
      setting_opt
        symbol_write_index_paths_file
        options.symbol_write_index_paths_file;
    symbol_write_index_paths_file_output =
      setting_opt
        symbol_write_index_paths_file_output
        options.symbol_write_index_paths_file_output;
    symbol_write_include_hhi =
      setting symbol_write_include_hhi options.symbol_write_include_hhi;
    symbol_write_sym_hash_in =
      setting_opt symbol_write_sym_hash_in options.symbol_write_sym_hash_in;
    symbol_write_exclude_out =
      setting_opt symbol_write_exclude_out options.symbol_write_exclude_out;
    symbol_write_referenced_out =
      setting_opt
        symbol_write_referenced_out
        options.symbol_write_referenced_out;
    symbol_write_reindexed_out =
      setting_opt symbol_write_reindexed_out options.symbol_write_reindexed_out;
    symbol_write_sym_hash_out =
      setting symbol_write_sym_hash_out options.symbol_write_sym_hash_out;
    tco_error_php_lambdas =
      setting tco_error_php_lambdas options.tco_error_php_lambdas;
    tco_disallow_discarded_nullable_awaitables =
      setting
        tco_disallow_discarded_nullable_awaitables
        options.tco_disallow_discarded_nullable_awaitables;
    tco_higher_kinded_types =
      setting tco_higher_kinded_types options.tco_higher_kinded_types;
    tco_report_pos_from_reason =
      setting tco_report_pos_from_reason options.tco_report_pos_from_reason;
    tco_typecheck_sample_rate =
      setting tco_typecheck_sample_rate options.tco_typecheck_sample_rate;
    tco_enable_sound_dynamic =
      setting tco_enable_sound_dynamic options.tco_enable_sound_dynamic;
    tco_pessimise_builtins =
      setting tco_pessimise_builtins options.tco_pessimise_builtins;
    tco_enable_no_auto_dynamic =
      setting tco_enable_no_auto_dynamic options.tco_enable_no_auto_dynamic;
    tco_skip_check_under_dynamic =
      setting tco_skip_check_under_dynamic options.tco_skip_check_under_dynamic;
    tco_global_access_check_enabled =
      setting
        tco_global_access_check_enabled
        options.tco_global_access_check_enabled;
    tco_enable_strict_string_concat_interp =
      setting
        tco_enable_strict_string_concat_interp
        options.tco_enable_strict_string_concat_interp;
    tco_ignore_unsafe_cast =
      setting tco_ignore_unsafe_cast options.tco_ignore_unsafe_cast;
    tco_enable_expression_trees =
      setting tco_enable_expression_trees options.tco_enable_expression_trees;
    tco_enable_function_references =
      setting
        tco_enable_function_references
        options.tco_enable_function_references;
    tco_allowed_expression_tree_visitors =
      setting
        tco_allowed_expression_tree_visitors
        options.tco_allowed_expression_tree_visitors;
    tco_typeconst_concrete_concrete_error =
      setting
        tco_typeconst_concrete_concrete_error
        options.tco_typeconst_concrete_concrete_error;
    tco_enable_strict_const_semantics =
      setting
        tco_enable_strict_const_semantics
        options.tco_enable_strict_const_semantics;
    tco_strict_wellformedness =
      setting tco_strict_wellformedness options.tco_strict_wellformedness;
    tco_meth_caller_only_public_visibility =
      setting
        tco_meth_caller_only_public_visibility
        options.tco_meth_caller_only_public_visibility;
    tco_require_extends_implements_ancestors =
      setting
        tco_require_extends_implements_ancestors
        options.tco_require_extends_implements_ancestors;
    tco_strict_value_equality =
      setting tco_strict_value_equality options.tco_strict_value_equality;
    tco_enforce_sealed_subclasses =
      setting
        tco_enforce_sealed_subclasses
        options.tco_enforce_sealed_subclasses;
    tco_implicit_inherit_sdt =
      setting tco_implicit_inherit_sdt options.tco_implicit_inherit_sdt;
    tco_explicit_consistent_constructors =
      setting
        tco_explicit_consistent_constructors
        options.tco_explicit_consistent_constructors;
    tco_require_types_class_consts =
      setting
        tco_require_types_class_consts
        options.tco_require_types_class_consts;
    tco_type_printer_fuel =
      setting tco_type_printer_fuel options.tco_type_printer_fuel;
    tco_specify_manifold_api_key =
      setting tco_specify_manifold_api_key options.tco_specify_manifold_api_key;
    tco_profile_top_level_definitions =
      setting
        tco_profile_top_level_definitions
        options.tco_profile_top_level_definitions;
    tco_allow_all_files_for_module_declarations =
      setting
        tco_allow_all_files_for_module_declarations
        options.tco_allow_all_files_for_module_declarations;
    tco_allowed_files_for_module_declarations =
      setting
        tco_allowed_files_for_module_declarations
        options.tco_allowed_files_for_module_declarations;
    tco_record_fine_grained_dependencies =
      setting
        tco_record_fine_grained_dependencies
        options.tco_record_fine_grained_dependencies;
    tco_loop_iteration_upper_bound =
      setting
        tco_loop_iteration_upper_bound
        options.tco_loop_iteration_upper_bound;
    tco_use_type_alias_heap =
      setting tco_use_type_alias_heap options.tco_use_type_alias_heap;
    tco_populate_dead_unsafe_cast_heap =
      setting
        tco_populate_dead_unsafe_cast_heap
        options.tco_populate_dead_unsafe_cast_heap;
    tco_rust_elab = setting tco_rust_elab options.tco_rust_elab;
    dump_tast_hashes = setting dump_tast_hashes options.dump_tast_hashes;
    dump_tasts = setting dump_tasts options.dump_tasts;
    tco_autocomplete_mode =
      setting tco_autocomplete_mode options.tco_autocomplete_mode;
    tco_package_info = setting tco_package_info options.tco_package_info;
    tco_log_exhaustivity_check =
      setting tco_log_exhaustivity_check options.tco_log_exhaustivity_check;
    tco_sticky_quarantine =
      setting tco_sticky_quarantine options.tco_sticky_quarantine;
    tco_lsp_invalidation =
      setting tco_lsp_invalidation options.tco_lsp_invalidation;
    tco_autocomplete_sort_text =
      setting tco_autocomplete_sort_text options.tco_autocomplete_sort_text;
    tco_extended_reasons =
      setting_opt tco_extended_reasons options.tco_extended_reasons;
    hack_warnings = setting hack_warnings options.hack_warnings;
    tco_strict_switch = setting tco_strict_switch options.tco_strict_switch;
    tco_allowed_files_for_ignore_readonly =
      setting
        tco_allowed_files_for_ignore_readonly
        options.tco_allowed_files_for_ignore_readonly;
    tco_package_v2 = setting tco_package_v2 options.tco_package_v2;
    tco_package_v2_bypass_package_check_for_class_const =
      setting
        tco_package_v2_bypass_package_check_for_class_const
        options.tco_package_v2_bypass_package_check_for_class_const;
    preexisting_warnings =
      setting preexisting_warnings options.preexisting_warnings;
    re_no_cache = setting re_no_cache options.re_no_cache;
    hh_distc_should_disable_trace_store =
      setting
        hh_distc_should_disable_trace_store
        options.hh_distc_should_disable_trace_store;
    tco_enable_abstract_method_optional_parameters =
      setting
        tco_enable_abstract_method_optional_parameters
        options.tco_enable_abstract_method_optional_parameters;
  }

let so_naming_sqlite_path t = t.so_naming_sqlite_path

let allowed_fixme_codes_strict t = t.allowed_fixme_codes_strict

let code_agnostic_fixme t = t.code_agnostic_fixme
