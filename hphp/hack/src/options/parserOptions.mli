(*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)
type t = {
  hhvm_compat_mode: bool;
  hhi_mode: bool;
  codegen: bool;  (** Are we emitting bytecode? *)
  disable_lval_as_an_expression: bool;
      (** Flag to disable using lvals as expressions. *)
  disable_legacy_soft_typehints: bool;
      (** Disable legacy soft typehint syntax (@int) and only allow the __Soft attribute. *)
  const_static_props: bool;  (** Enable const static properties *)
  disable_legacy_attribute_syntax: bool;
      (** Disable <<...>> attribute syntax *)
  const_default_func_args: bool;
      (** Statically check default function arguments *)
  abstract_static_props: bool;  (** Static properties can be abstract *)
  disallow_func_ptrs_in_constants: bool;
      (** Flag to disallow HH\fun and HH\class_meth in constants and constant initializers *)
  enable_xhp_class_modifier: bool;
      (** Enable the new style xhp class.
         Old style: class :name {}
         New style: xhp class name {} *)
  disable_xhp_element_mangling: bool;
      (** Flag to disable the old stype xhp element mangling. `<something/>` would otherwise be resolved as `xhp_something`
         The new style `xhp class something {}` does not do this style of mangling, thus we need a way to disable it on the
         'lookup side'. *)
  disable_xhp_children_declarations: bool;
      (** Disable `children (foo|bar+|pcdata)` declarations as they can be implemented without special syntax *)
  const_default_lambda_args: bool;
      (** Statically check default lambda arguments. Subset of default_func_args *)
  allow_unstable_features: bool;
      (** Allows enabling unstable features via the __EnableUnstableFeatures attribute *)
  interpret_soft_types_as_like_types: bool;  (** <<__Soft>> T -> ~T *)
  is_systemlib: bool;  (** Enable features used to typecheck systemlib *)
  disallow_static_constants_in_default_func_args: bool;
  disallow_direct_superglobals_refs: bool;
      (** block accessing superglobals via their variable names *)
  auto_namespace_map: (string * string) list;  (** Namespace aliasing map *)
  everything_sdt: bool;
      (** All classes are implcitly marked <<__SupportDynamicType>> *)
  keep_user_attributes: bool;
      (** Parse all user attributes rather than only the ones needed for typing *)
  stack_size: int;
      (** Stack size to use for parallel workers inside the parser. *)
  deregister_php_stdlib: bool;
      (** Flag for disabling functions in HHI files with the __PHPStdLib attribute *)
  enable_class_level_where_clauses: bool;
      (** Enable class-level where clauses, i.e. class base<T> where T = int {} *)
  union_intersection_type_hints: bool;
      (** Enables union and intersection type hints *)
  unwrap_concurrent: bool;
      (** Replace concurrent blocks with their bodies in the AST *)
  disallow_silence: bool;  (** Flag to disable the error suppression operator *)
  no_parser_readonly_check: bool;  (** Disable parser-based readonly checking *)
  parser_errors_only: bool;
      (** Ignore all errors except those that can influence the shape of syntax tree
         (skipping post parse error checks) *)
  disable_hh_ignore_error: int;
      (** Disable HH_IGNORE_ERROR comments, either raising an error if 1 or treating them as normal comments if 2. *)
  allowed_decl_fixme_codes: ISet.t;
      (** Set of error codes disallowed in decl positions *)
}
[@@deriving show, eq]

val default : t

(* Changes here need to be synchronized with rust_parser_errors_ffi.rs *)
type ffi_t =
  bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool
  * bool

val to_rust_ffi_t : t -> ffi_t
