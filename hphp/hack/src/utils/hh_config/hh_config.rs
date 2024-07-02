// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.
mod local_config;

use std::collections::BTreeMap;
use std::fs::File;
use std::io::BufRead;
use std::io::BufReader;
use std::path::Path;
use std::path::PathBuf;
use std::str::FromStr;

use anyhow::Context;
use anyhow::Result;
use config_file::ConfigFile;
pub use local_config::LocalConfig;
use oxidized::custom_error_config::CustomErrorConfig;
use oxidized::decl_parser_options::DeclParserOptions;
use oxidized::global_options::ExtendedReasonsConfig;
use oxidized::global_options::GlobalOptions;
use oxidized::parser_options::ParserOptions;
use package::PackageInfo;
use sha1::Digest;
use sha1::Sha1;

pub const FILE_PATH_RELATIVE_TO_ROOT: &str = ".hhconfig";
pub const PACKAGE_FILE_PATH_RELATIVE_TO_ROOT: &str = "PACKAGES.toml";

/// For now, this struct only contains the parts of .hhconfig which
/// have been needed in Rust tools.
///
/// Fields correspond to ServerConfig.t
#[derive(Debug, Clone, Default)]
pub struct HhConfig {
    pub version: Option<String>,

    /// List of regex patterns of root-relative paths to ignore.
    pub ignored_paths: Vec<String>,

    /// SHA1 Hash of the .hhconfig file contents.
    pub hash: String,

    pub opts: GlobalOptions,
    pub local_config: LocalConfig,

    pub gc_minor_heap_size: usize,
    pub gc_space_overhead: usize,
    pub hackfmt_version: usize,
    pub sharedmem_dep_table_pow: usize,
    pub sharedmem_global_size: usize,
    pub sharedmem_hash_table_pow: usize,
    pub sharedmem_heap_size: usize,
    pub ide_fall_back_to_full_index: bool,
    pub hh_distc_should_disable_trace_store: bool,
    pub naming_table_compression_level: usize,
    pub naming_table_compression_threads: usize,
    pub eden_fetch_parallelism: usize,
}

impl HhConfig {
    pub fn from_root(root: impl AsRef<Path>, overrides: &ConfigFile) -> Result<Self> {
        let hhconfig_path = root.as_ref().join(FILE_PATH_RELATIVE_TO_ROOT);
        let hh_conf_path = system_config_path();
        Self::from_files(root, hhconfig_path, hh_conf_path, overrides)
    }

    pub fn create_packages_path(hhconfig_path: &Path) -> PathBuf {
        // Unwrap is safe because hhconfig_path is always at least one nonempty string
        let mut packages_path = hhconfig_path.parent().unwrap().to_path_buf();
        packages_path.push("PACKAGES.toml");
        packages_path
    }

    pub fn create_custom_errors_path(hhconfig_path: &Path) -> PathBuf {
        // Unwrap is safe because hhconfig_path is always at least one nonempty string
        let mut packages_path = hhconfig_path.parent().unwrap().to_path_buf();
        packages_path.push("CUSTOM_ERRORS.json");
        packages_path
    }

    pub fn from_files(
        root: impl AsRef<Path>,
        hhconfig_path: impl AsRef<Path>,
        hh_conf_path: impl AsRef<Path>,
        overrides: &ConfigFile,
    ) -> Result<Self> {
        let hhconfig_path = hhconfig_path.as_ref();
        let package_config_pathbuf = Self::create_packages_path(hhconfig_path);
        let package_config_path = package_config_pathbuf.as_path();
        let custom_error_config_path = Self::create_custom_errors_path(hhconfig_path);
        let (contents, mut hhconfig) = ConfigFile::from_file_with_contents(hhconfig_path)
            .with_context(|| hhconfig_path.display().to_string())?;
        // Grab extra config and use it to process the hash
        let package_contents: String = if package_config_path.exists() {
            let ctxt = || package_config_path.display().to_string();
            let bytes = std::fs::read(package_config_path).with_context(ctxt)?;
            String::from_utf8(bytes).unwrap()
        } else {
            String::new()
        };
        let package_info: PackageInfo = PackageInfo::from_text(
            root.as_ref().to_str().unwrap_or_default(),
            package_config_pathbuf.to_str().unwrap_or_default(),
        )
        .unwrap_or_default();
        let custom_error_contents: String = if custom_error_config_path.exists() {
            let ctxt = || custom_error_config_path.as_path().display().to_string();
            let bytes = std::fs::read(&custom_error_config_path).with_context(ctxt)?;
            String::from_utf8(bytes).unwrap()
        } else {
            "[]".to_string()
        };
        let hash = Self::hash(
            &hhconfig,
            &contents,
            &package_contents,
            &custom_error_contents,
        );
        hhconfig.apply_overrides(overrides);
        let hh_conf_path = hh_conf_path.as_ref();

        // We don't try to gracefully handle the case where the /etc/hh.conf
        // (or overridden path) does not exist on disk.
        //
        // There is no benefit: if someone makes a mistake with machine
        // configurations, we rather have everything blow up in our face fast
        // and tell us plainly what is going wrong than doing something "intelligent"
        // and falling back to some default configuration, which is bound to lead to
        // symptoms that are difficult to debug.
        let mut hh_conf = ConfigFile::from_file(hh_conf_path)
            .with_context(|| hh_conf_path.display().to_string())?;

        hh_conf.apply_overrides(overrides);
        let custom_error_config =
            CustomErrorConfig::from_str(&custom_error_contents).unwrap_or_default();
        Ok(Self {
            hash,
            ..Self::from_configs(hhconfig, hh_conf, custom_error_config, package_info)?
        })
    }

    pub fn into_config_files(
        root: impl AsRef<Path>,
    ) -> Result<(ConfigFile, ConfigFile, CustomErrorConfig, PackageInfo)> {
        let hhconfig_path = root.as_ref().join(FILE_PATH_RELATIVE_TO_ROOT);
        let hh_conf_path = system_config_path();
        let custom_error_config_pathbuf = Self::create_custom_errors_path(hhconfig_path.as_path());
        let custom_error_config_path = custom_error_config_pathbuf.as_path();
        let hh_config_file = ConfigFile::from_file(&hhconfig_path)
            .with_context(|| hhconfig_path.display().to_string())?;
        let hh_conf_file = ConfigFile::from_file(&hh_conf_path)
            .with_context(|| hh_conf_path.display().to_string())?;
        let custom_error_config = CustomErrorConfig::from_path(custom_error_config_path)?;
        let package_config_pathbuf = Self::create_packages_path(hhconfig_path.as_path());
        let package_config_path = package_config_pathbuf.as_path();
        let package_info: PackageInfo = PackageInfo::from_text(
            root.as_ref().to_str().unwrap_or_default(),
            package_config_path.to_str().unwrap_or_default(),
        )
        .unwrap_or_default();

        Ok((
            hh_conf_file,
            hh_config_file,
            custom_error_config,
            package_info,
        ))
    }

    fn hash(
        parsed: &ConfigFile,
        config_contents: &str,
        package_config: &str,
        custom_error_config: &str,
    ) -> String {
        if let Some(hash) = parsed.get_str("override_hhconfig_hash") {
            return hash.to_owned();
        }
        let mut hasher = Sha1::new();
        hasher.update(config_contents.as_bytes());
        hasher.update(package_config.as_bytes());
        hasher.update(custom_error_config.as_bytes());
        format!("{:x}", hasher.finalize())
    }

    pub fn from_slice(bytes: &[u8]) -> Result<Self> {
        let (hash, config) = ConfigFile::from_slice_with_sha1(bytes);
        Ok(Self {
            hash,
            ..Self::from_configs(
                config,
                Default::default(),
                Default::default(),
                PackageInfo::default(),
            )?
        })
    }

    /// Construct from .hhconfig and hh.conf files with CLI overrides already applied.
    pub fn from_configs(
        hhconfig: ConfigFile,
        hh_conf: ConfigFile,
        custom_error_config: CustomErrorConfig,
        package_info: PackageInfo,
    ) -> Result<Self> {
        let current_rolled_out_flag_idx = hhconfig
            .get_int("current_saved_state_rollout_flag_index")
            .unwrap_or(Ok(0))?;
        let deactivate_saved_state_rollout = hhconfig
            .get_bool("deactivate_saved_state_rollout")
            .unwrap_or(Ok(false))?;

        let version = hhconfig.get_str("version");

        let local_config = LocalConfig::from_config(
            version,
            current_rolled_out_flag_idx,
            deactivate_saved_state_rollout,
            &hh_conf,
        )?;

        let default = ParserOptions::default();
        let po = ParserOptions {
            hhvm_compat_mode: default.hhvm_compat_mode,
            hhi_mode: default.hhi_mode,
            auto_namespace_map: match hhconfig.get_str("auto_namespace_map") {
                None => default.auto_namespace_map,
                Some(s) => parse_json::<BTreeMap<String, String>>(s)?
                    .into_iter()
                    .collect(),
            },
            codegen: hhconfig.get_bool_or("codegen", default.codegen)?,
            deregister_php_stdlib: hhconfig
                .get_bool_or("deregister_php_stdlib", default.deregister_php_stdlib)?,
            allow_unstable_features: local_config.allow_unstable_features,
            disable_lval_as_an_expression: default.disable_lval_as_an_expression,
            union_intersection_type_hints: hhconfig.get_bool_or(
                "union_intersection_type_hints",
                default.union_intersection_type_hints,
            )?,
            enable_class_level_where_clauses: default.enable_class_level_where_clauses,
            disable_legacy_soft_typehints: default.disable_legacy_soft_typehints,
            allowed_decl_fixme_codes: hhconfig
                .get_int_set_or("allowed_decl_fixme_codes", default.allowed_decl_fixme_codes)?,
            const_static_props: default.const_static_props,
            disable_legacy_attribute_syntax: default.disable_legacy_attribute_syntax,
            const_default_func_args: hhconfig
                .get_bool_or("const_default_func_args", default.const_default_func_args)?,
            const_default_lambda_args: hhconfig.get_bool_or(
                "const_default_lambda_args",
                default.const_default_lambda_args,
            )?,
            disallow_silence: hhconfig.get_bool_or("disallow_silence", default.disallow_silence)?,
            abstract_static_props: default.abstract_static_props,
            parser_errors_only: default.parser_errors_only,
            disallow_func_ptrs_in_constants: hhconfig.get_bool_or(
                "disallow_func_ptrs_in_constants",
                default.disallow_func_ptrs_in_constants,
            )?,
            enable_xhp_class_modifier: hhconfig.get_bool_or(
                "enable_xhp_class_modifier",
                default.enable_xhp_class_modifier,
            )?,
            disable_xhp_element_mangling: hhconfig.get_bool_or(
                "disable_xhp_element_mangling",
                default.disable_xhp_element_mangling,
            )?,
            disable_xhp_children_declarations: hhconfig.get_bool_or(
                "disable_xhp_children_declarations",
                default.disable_xhp_children_declarations,
            )?,
            disable_hh_ignore_error: hhconfig
                .get_int_or("disable_hh_ignore_error", default.disable_hh_ignore_error)?,
            keep_user_attributes: default.keep_user_attributes,
            is_systemlib: default.is_systemlib,
            interpret_soft_types_as_like_types: hhconfig.get_bool_or(
                "interpret_soft_types_as_like_types",
                default.interpret_soft_types_as_like_types,
            )?,
            no_parser_readonly_check: default.no_parser_readonly_check,
            everything_sdt: hhconfig.get_bool_or("everything_sdt", default.everything_sdt)?,
            disallow_static_constants_in_default_func_args: default
                .disallow_static_constants_in_default_func_args,
            unwrap_concurrent: default.unwrap_concurrent,
            disallow_direct_superglobals_refs: hhconfig.get_bool_or(
                "disallow_direct_superglobals_refs",
                default.disallow_direct_superglobals_refs,
            )?,
            stack_size: default.stack_size,
        };
        let default = GlobalOptions::default();
        let opts = GlobalOptions {
            po,
            tco_saved_state: local_config.saved_state.clone(),
            tco_experimental_features: hhconfig
                .get_string_set_or("enable_experimental_tc_features", default.tco_experimental_features),
            tco_migration_flags: default.tco_migration_flags,
            tco_num_local_workers: default.tco_num_local_workers,
            tco_defer_class_declaration_threshold: default.tco_defer_class_declaration_threshold,
            tco_locl_cache_capacity: hhconfig.get_int_or("locl_cache_capacity", default.tco_locl_cache_capacity)?,
            tco_locl_cache_node_threshold: hhconfig.get_int_or("locl_cache_node_threshold", default.tco_locl_cache_node_threshold)?,
            so_naming_sqlite_path: default.so_naming_sqlite_path,
            po_disallow_toplevel_requires: hhconfig.get_bool_or("disallow_toplevel_requires", default.po_disallow_toplevel_requires)?,
            tco_log_large_fanouts_threshold: default.tco_log_large_fanouts_threshold,
            tco_log_inference_constraints: default.tco_log_inference_constraints,
            tco_language_feature_logging: default.tco_language_feature_logging,
            tco_timeout: hhconfig.get_int_or("timeout", default.tco_timeout)?,
            tco_disallow_invalid_arraykey: hhconfig.get_bool_or("disallow_invalid_arraykey", default.tco_disallow_invalid_arraykey)?,
            code_agnostic_fixme: hhconfig.get_bool_or("code_agnostic_fixme", default.code_agnostic_fixme)?,
            allowed_fixme_codes_strict: hhconfig.get_int_set_or("allowed_fixme_codes_strict", default.allowed_fixme_codes_strict)?,
            log_levels: hhconfig.get_str("log_levels").map_or(Ok(default.log_levels), parse_json)?,
            class_pointer_levels: hhconfig.get_str("class_pointer_levels").map_or(Ok(default.class_pointer_levels), parse_json)?,
            tco_remote_old_decls_no_limit: default.tco_remote_old_decls_no_limit,
            tco_use_old_decls_from_cas: default.tco_use_old_decls_from_cas,
            tco_fetch_remote_old_decls: default.tco_fetch_remote_old_decls,
            tco_populate_member_heaps: default.tco_populate_member_heaps,
            tco_skip_hierarchy_checks: default.tco_skip_hierarchy_checks,
            tco_skip_tast_checks: default.tco_skip_tast_checks,
            tco_like_type_hints: hhconfig.get_bool_or("like_type_hints", default.tco_like_type_hints)?,
            tco_coeffects: default.tco_coeffects,
            tco_coeffects_local: default.tco_coeffects_local,
            tco_strict_contexts: default.tco_strict_contexts,
            tco_like_casts: hhconfig.get_bool_or("like_casts", default.tco_like_casts)?,
            tco_check_xhp_attribute: hhconfig.get_bool_or("check_xhp_attribute", default.tco_check_xhp_attribute)?,
            tco_check_redundant_generics: hhconfig.get_bool_or("check_redundant_generics", default.tco_check_redundant_generics)?,
            tco_disallow_unresolved_type_variables: default
                .tco_disallow_unresolved_type_variables,
            tco_custom_error_config: custom_error_config,
            tco_const_attribute: default.tco_const_attribute,
            tco_check_attribute_locations: default.tco_check_attribute_locations,
            glean_reponame: default.glean_reponame,
            symbol_write_index_inherited_members: default.symbol_write_index_inherited_members,
            symbol_write_ownership: default.symbol_write_ownership,
            symbol_write_root_path: default.symbol_write_root_path,
            symbol_write_hhi_path: default.symbol_write_hhi_path,
            symbol_write_ignore_paths: default.symbol_write_ignore_paths,
            symbol_write_index_paths: default.symbol_write_index_paths,
            symbol_write_index_paths_file: default.symbol_write_index_paths_file,
            symbol_write_index_paths_file_output: default.symbol_write_index_paths_file_output,
            symbol_write_include_hhi: default.symbol_write_include_hhi,
            symbol_write_sym_hash_in: default.symbol_write_sym_hash_in,
            symbol_write_exclude_out: default.symbol_write_exclude_out,
            symbol_write_referenced_out: default.symbol_write_referenced_out,
            symbol_write_reindexed_out: default.symbol_write_reindexed_out,
            symbol_write_sym_hash_out: default.symbol_write_sym_hash_out,
            tco_error_php_lambdas: default.tco_error_php_lambdas,
            tco_disallow_discarded_nullable_awaitables: default
                .tco_disallow_discarded_nullable_awaitables,
            tco_higher_kinded_types: default.tco_higher_kinded_types,
            tco_report_pos_from_reason: default.tco_report_pos_from_reason,
            tco_typecheck_sample_rate: hhconfig.get_float_or("typecheck_sample_rate", default.tco_typecheck_sample_rate)?,
            tco_enable_sound_dynamic: hhconfig.get_bool_or("enable_sound_dynamic_type", default.tco_enable_sound_dynamic)?,
            tco_pessimise_builtins: hhconfig.get_bool_or("pessimise_builtins", default.tco_pessimise_builtins)?,
            tco_enable_no_auto_dynamic: hhconfig.get_bool_or("enable_no_auto_dynamic", default.tco_enable_no_auto_dynamic)?,
            tco_skip_check_under_dynamic: hhconfig.get_bool_or("skip_check_under_dynamic", default.tco_skip_check_under_dynamic)?,
            tco_global_access_check_enabled: hhconfig.get_bool_or("tco_global_access_check_enabled", default.tco_global_access_check_enabled)?,
            tco_enable_strict_string_concat_interp: hhconfig.get_bool_or("enable_strict_string_concat_interp", default
                .tco_enable_strict_string_concat_interp)?,
            tco_ignore_unsafe_cast: default.tco_ignore_unsafe_cast,
            tco_enable_expression_trees: default.tco_enable_expression_trees,
            tco_enable_function_references: hhconfig.get_bool_or("enable_function_references", default.tco_enable_function_references)?,
            tco_allowed_expression_tree_visitors:
                hhconfig
                    .get_str("allowed_expression_tree_visitors")
                    .map_or(default.tco_allowed_expression_tree_visitors, |s| {
                        let mut allowed_expression_tree_visitors = parse_svec(s);
                        // Fix up type names so they will match with elaborated names.
                        // Keep this in sync with the Utils.add_ns loop in server/serverConfig.ml
                        for ty in &mut allowed_expression_tree_visitors {
                            if !ty.starts_with('\\') {
                                *ty = format!("\\{}", ty)
                            }
                        };
                        allowed_expression_tree_visitors
                    }),
            tco_typeconst_concrete_concrete_error: default.tco_typeconst_concrete_concrete_error,
            tco_enable_strict_const_semantics: hhconfig.get_int_or("enable_strict_const_semantics", default.tco_enable_strict_const_semantics)?,
            tco_strict_wellformedness: hhconfig.get_int_or("strict_wellformedness", default.tco_strict_wellformedness)?,
            tco_meth_caller_only_public_visibility: default
                .tco_meth_caller_only_public_visibility,
            tco_require_extends_implements_ancestors: default
                .tco_require_extends_implements_ancestors,
            tco_strict_value_equality: default.tco_strict_value_equality,
            tco_enforce_sealed_subclasses: default.tco_enforce_sealed_subclasses,
            tco_implicit_inherit_sdt: default.tco_implicit_inherit_sdt,
            tco_explicit_consistent_constructors: hhconfig.get_int_or("explicit_consistent_constructors", default.tco_explicit_consistent_constructors)?,
            tco_require_types_class_consts: hhconfig.get_int_or("require_types_tco_require_types_class_consts", default.tco_require_types_class_consts)?,
            tco_type_printer_fuel: hhconfig.get_int_or("type_printer_fuel", default.tco_type_printer_fuel)?,
            tco_specify_manifold_api_key: default.tco_specify_manifold_api_key,
            tco_profile_top_level_definitions: hhconfig.get_bool_or("profile_top_level_definitions", default.tco_profile_top_level_definitions)?,
            tco_allow_all_files_for_module_declarations: default
                .tco_allow_all_files_for_module_declarations,
            tco_allowed_files_for_module_declarations: hhconfig.get_str("allowed_files_for_module_declarations").map_or(default.tco_allowed_files_for_module_declarations, parse_svec),
            tco_allowed_files_for_ignore_readonly: hhconfig.get_str("allowed_files_for_ignore_readonly").map_or(default.tco_allowed_files_for_ignore_readonly, parse_svec),
            tco_record_fine_grained_dependencies: default.tco_record_fine_grained_dependencies,
            tco_loop_iteration_upper_bound: default.tco_loop_iteration_upper_bound,
            tco_use_type_alias_heap: default.tco_use_type_alias_heap,
            tco_populate_dead_unsafe_cast_heap: default.tco_populate_dead_unsafe_cast_heap,
            tco_rust_elab: local_config.rust_elab,
            dump_tast_hashes: hh_conf.get_bool_or("dump_tast_hashes", default.dump_tast_hashes)?,
            dump_tasts: match hh_conf.get_str("dump_tasts") {
                None => default.dump_tasts,
                Some(path) => {
                    let path = PathBuf::from(path);
                    let file =
                        File::open(&path).with_context(|| path.to_string_lossy().to_string())?;
                    BufReader::new(file)
                        .lines()
                        .collect::<std::io::Result<_>>()?
                }
            },
            tco_autocomplete_mode: default.tco_autocomplete_mode,
            tco_package_info:
                // If there are errors, ignore them for the tcopt, the parser errors will be caught and
                // sent separately.
                package_info.try_into().unwrap_or_default(),
            tco_log_exhaustivity_check: hhconfig.get_bool_or("log_exhaustivity_check", default.tco_log_exhaustivity_check)?,
            tco_sticky_quarantine: default.tco_sticky_quarantine,
            tco_lsp_invalidation: default.tco_lsp_invalidation,
            tco_autocomplete_sort_text: default.tco_autocomplete_sort_text,
            tco_extended_reasons:  hhconfig.get_either_int_or_str("extended_reasons").and_then(|res| match res  {
                Ok (n) => Some(ExtendedReasonsConfig::Extended(n)),
                Err(data) => {
                    if data.eq("debug") { Some(ExtendedReasonsConfig::Debug) }
                    else if data.eq("yolo") { Some(ExtendedReasonsConfig::Yolo) }
                    else { None }
                }
            }),
            hack_warnings: hhconfig.get_all_or_some_ints_or("hack_warnings", default.hack_warnings)?,
            tco_strict_switch: hhconfig.get_bool_or("strict_switch", default.tco_strict_switch)?,
            tco_package_v2: hhconfig.get_bool_or("package_v2", default.tco_package_v2)?,
            tco_package_v2_bypass_package_check_for_class_const: hhconfig.get_bool_or("package_v2_bypass_package_check_for_class_const", default.tco_package_v2_bypass_package_check_for_class_const)?,
            preexisting_warnings: default.preexisting_warnings,
            re_no_cache: hhconfig.get_bool_or("re_no_cache", default.re_no_cache)?,
            hh_distc_should_disable_trace_store: hhconfig.get_bool_or(
                    "hh_distc_should_disable_trace_store", default.hh_distc_should_disable_trace_store)?,
            tco_enable_abstract_method_optional_parameters: hhconfig.get_bool_or("enable_abstract_method_optional_parameters", default.tco_enable_abstract_method_optional_parameters)?,
        };
        let mut c = Self {
            local_config,
            opts,
            ..Self::default()
        };

        for (key, mut value) in hhconfig {
            match key.as_str() {
                "version" => {
                    c.version = Some(value);
                }
                "ignored_paths" => {
                    c.ignored_paths = parse_json(&value)?;
                }
                "gc_minor_heap_size" => {
                    value.retain(|c| c != '_');
                    c.gc_minor_heap_size = parse_json(&value)?;
                }
                "gc_space_overhead" => {
                    c.gc_space_overhead = parse_json(&value)?;
                }
                "hackfmt.version" => {
                    c.hackfmt_version = parse_json(&value)?;
                }
                "sharedmem_dep_table_pow" => {
                    c.sharedmem_dep_table_pow = parse_json(&value)?;
                }
                "sharedmem_global_size" => {
                    value.retain(|c| c != '_');
                    c.sharedmem_global_size = parse_json(&value)?;
                }
                "sharedmem_hash_table_pow" => {
                    c.sharedmem_hash_table_pow = parse_json(&value)?;
                }
                "sharedmem_heap_size" => {
                    value.retain(|c| c != '_');
                    c.sharedmem_heap_size = parse_json(&value)?;
                }
                "ide_fall_back_to_full_index" => {
                    c.ide_fall_back_to_full_index = parse_json(&value)?;
                }
                "hh_distc_should_disable_trace_store" => {
                    c.hh_distc_should_disable_trace_store = parse_json(&value)?;
                }
                "naming_table_compression_threads" => {
                    c.naming_table_compression_threads = parse_json(&value)?;
                }
                "naming_table_compression_level" => {
                    c.naming_table_compression_level = parse_json(&value)?;
                }
                "eden_fetch_parallelism" => {
                    c.eden_fetch_parallelism = parse_json(&value)?;
                }
                _ => {}
            }
        }
        Ok(c)
    }

    pub fn get_decl_parser_options(&self) -> DeclParserOptions {
        DeclParserOptions::from_parser_options(&self.opts.po)
    }
}

fn parse_json<'de, T: serde::de::Deserialize<'de>>(value: &'de str) -> Result<T> {
    Ok(serde_json::from_slice(value.as_bytes())?)
}

fn parse_svec(value: &str) -> Vec<String> {
    value
        .split_terminator(',')
        .map(|s| s.trim().to_owned())
        .collect()
}

/// Return the local config file path, allowing HH_LOCALCONF_PATH to override it.
pub fn system_config_path() -> PathBuf {
    const HH_CONF: &str = "hh.conf";
    match std::env::var_os("HH_LOCALCONF_PATH") {
        Some(path) => Path::new(&path).join(HH_CONF),
        None => Path::new("/etc").join(HH_CONF), // TODO see options/buildOptions.ml for mac cfg
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_log_levels() {
        let hhconf = HhConfig::from_slice(br#"log_levels={ "pessimise": 1 }"#).unwrap();
        assert_eq!(
            hhconf.opts.log_levels.get("pessimise").copied(),
            Some(1isize)
        );
    }
}
