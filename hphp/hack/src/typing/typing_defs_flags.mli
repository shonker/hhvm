(*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

(*
 * Copyright (c) 2015, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

module Fun : sig
  type t [@@deriving eq, hash, ord, show]

  type record = {
    return_disposable: bool;
    async: bool;
    generator: bool;
    fun_kind: Ast_defs.fun_kind;
    instantiated_targs: bool;
    is_function_pointer: bool;
    returns_readonly: bool;
    readonly_this: bool;
    support_dynamic_type: bool;
    is_memoized: bool;
    variadic: bool;
  }

  val return_disposable : t -> bool

  val set_return_disposable : bool -> t -> t

  val async : t -> bool

  val set_async : bool -> t -> t

  val generator : t -> bool

  val set_generator : bool -> t -> t

  val instantiated_targs : t -> bool

  val set_instantiated_targs : bool -> t -> t

  val is_function_pointer : t -> bool

  val set_is_function_pointer : bool -> t -> t

  val returns_readonly : t -> bool

  val set_returns_readonly : bool -> t -> t

  (** Does function captures only readonly values?
   *  For methods: is the this parameter readonly?
   *)
  val readonly_this : t -> bool

  (** Mark the function type as capturing only readonly values,
   *  or for method definitions, the this parameter is readonly *)
  val set_readonly_this : bool -> t -> t

  val support_dynamic_type : t -> bool

  val set_support_dynamic_type : bool -> t -> t

  val is_memoized : t -> bool

  val set_is_memoized : bool -> t -> t

  val variadic : t -> bool

  val set_variadic : bool -> t -> t

  val fun_kind : t -> Ast_defs.fun_kind

  val make :
    Ast_defs.fun_kind ->
    return_disposable:bool ->
    returns_readonly:bool ->
    readonly_this:bool ->
    support_dynamic_type:bool ->
    is_memoized:bool ->
    variadic:bool ->
    t

  val default : t

  val as_record : t -> record
end

module FunParam : sig
  type t [@@deriving eq, hash, ord, show]

  type record = {
    accept_disposable: bool;
    inout: bool;
    has_default: bool;
    readonly: bool;
    ignore_readonly_error: bool;
  }

  val accept_disposable : t -> bool

  val inout : t -> bool

  val has_default : t -> bool

  val readonly : t -> bool

  val ignore_readonly_error : t -> bool

  val set_accept_disposable : bool -> t -> t

  val set_inout : bool -> t -> t

  val set_has_default : bool -> t -> t

  val set_readonly : bool -> t -> t

  val set_ignore_readonly_error : bool -> t -> t

  val make :
    inout:bool ->
    accept_disposable:bool ->
    has_default:bool ->
    readonly:bool ->
    ignore_readonly_error:bool ->
    t

  val as_record : t -> record

  val default : t
end

module ClassElt : sig
  type t [@@deriving show]

  val make :
    xhp_attr:Xhp_attribute.t option ->
    abstract:bool ->
    final:bool ->
    superfluous_override:bool ->
    lsb:bool ->
    synthesized:bool ->
    const:bool ->
    lateinit:bool ->
    dynamicallycallable:bool ->
    readonly_prop:bool ->
    support_dynamic_type:bool ->
    needs_init:bool ->
    safe_global_variable:bool ->
    t

  val is_abstract : t -> bool

  val is_final : t -> bool

  val has_superfluous_override : t -> bool

  val has_lsb : t -> bool

  (** Whether a class element comes from a `require extends`. *)
  val is_synthesized : t -> bool

  val is_const : t -> bool

  val has_lateinit : t -> bool

  val is_dynamicallycallable : t -> bool

  val supports_dynamic_type : t -> bool

  val is_readonly_prop : t -> bool

  val needs_init : t -> bool

  val is_safe_global_variable : t -> bool

  val get_xhp_attr : t -> Xhp_attribute.t option

  val set_synthesized : t -> t

  val reset_superfluous_override : t -> t
end
