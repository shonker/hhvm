(*
 * Copyright (c) 2015, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

(* This module implements the typing.
 *
 * Given an Nast.program, it infers the type of all the local
 * variables, and checks that all the types are correct (aka
 * consistent) *)
open Hh_prelude
open Common
open Typing_defs
open Typing_env_types
open Aast
module Env = Typing_env
module SN = Naming_special_names
module Inter = Typing_intersection
module Union = Typing_union
module MakeType = Typing_make_type
module Utils = Typing_utils

module ExpectedTy : sig
  [@@@warning "-32"]

  type t = private {
    pos: Pos.t;
    reason: Typing_reason.ureason;
    ty: locl_ty;
    coerce: Typing_logic.coercion_direction option;
  }
  [@@deriving show]

  [@@@warning "+32"]

  val make :
    ?coerce:Typing_logic.coercion_direction option ->
    Pos.t ->
    Typing_reason.ureason ->
    locl_ty ->
    t

  (* We will allow coercion to this expected type, if et_enforced=Enforced *)
  val make_and_allow_coercion : Pos.t -> Typing_reason.ureason -> locl_ty -> t

  (* If type is an unsolved type variable, don't create an expected type *)
  val make_and_allow_coercion_opt :
    Typing_env_types.env ->
    Pos.t ->
    Typing_reason.ureason ->
    locl_ty ->
    t option
end = struct
  (* Some mutually recursive inference functions in typing.ml pass around an ~expected argument that
   * enables bidirectional type checking. This module abstracts away that type so that it can be
   * extended and modified without having to touch every consumer. *)
  type t = {
    pos: Pos.t;
    reason: Typing_reason.ureason;
    ty: locl_ty;
    coerce: Typing_logic.coercion_direction option;
  }
  [@@deriving show]

  let make_and_allow_coercion_opt env pos reason ty =
    let (_env, ety) = Env.expand_type env ty in
    match get_node ety with
    | Tvar v when Internal_type_set.is_empty (Env.get_tyvar_upper_bounds env v)
      ->
      None
    | _ -> Some { pos; reason; ty; coerce = None }

  let make_and_allow_coercion pos reason ty = { pos; reason; ty; coerce = None }

  let make ?coerce pos reason locl_ty =
    let res = make_and_allow_coercion pos reason locl_ty in
    match coerce with
    | None -> res
    | Some coerce -> { res with coerce }
end

let decl_error_to_typing_error decl_error =
  let primary =
    match decl_error with
    | Decl_defs.Wrong_extend_kind
        { pos; kind; name; parent_pos; parent_kind; parent_name } ->
      Typing_error.Primary.Wrong_extend_kind
        { pos; kind; name; parent_pos; parent_kind; parent_name }
    | Decl_defs.Wrong_use_kind { pos; name; parent_pos; parent_name } ->
      Typing_error.Primary.Wrong_use_kind { pos; name; parent_pos; parent_name }
    | Decl_defs.Cyclic_class_def { pos; stack } ->
      Typing_error.Primary.Cyclic_class_def { pos; stack }
  in
  Typing_error.primary primary

let add_decl_error decl_error ~env =
  Typing_error_utils.add_typing_error
    ~env
    (decl_error_to_typing_error decl_error)

let add_decl_errors ~env = List.iter ~f:(add_decl_error ~env)

(*****************************************************************************)
(* Handling function/method arguments *)
(*****************************************************************************)
let param_has_attribute param attr =
  List.exists param.param_user_attributes ~f:(fun { ua_name; _ } ->
      String.equal attr (snd ua_name))

let has_accept_disposable_attribute param =
  param_has_attribute param SN.UserAttributes.uaAcceptDisposable

let with_timeout env fun_name (do_ : env -> 'b) : 'b option =
  let timeout = TypecheckerOptions.(timeout @@ Env.get_tcopt env) in
  if Int.equal timeout 0 then
    Some (do_ env)
  else
    let big_envs = ref [] in
    let env = { env with big_envs } in
    Timeout.with_timeout
      ~timeout
      ~on_timeout:(fun _ ->
        (*List.iter !big_envs (fun (p, env) ->
            Typing_log.log_key "WARN: environment is too big.";
            Typing_log.hh_show_env p env); *)
        let (pos, fn_name) = fun_name in
        Errors.typechecker_timeout pos fn_name timeout;
        None)
      ~do_:(fun _ -> Some (do_ env))

(* If the localized types of the return type is a tyvar we force it to be covariant.
   The same goes for parameter types, but this time we force them to be contravariant
*)
let set_tyvars_variance_in_callable env return_ty param_tys =
  Env.log_env_change "set_tyvars_variance_in_callable" env
  @@
  let set_variance = Env.set_tyvar_variance ~for_all_vars:true in
  let env = set_variance env return_ty in
  let env =
    List.fold param_tys ~init:env ~f:(fun env opt_ty ->
        match opt_ty with
        | None -> env
        | Some ty -> set_variance ~flip:true env ty)
  in
  env

let reify_kind = function
  | Erased -> Aast.Erased
  | SoftReified -> Aast.SoftReified
  | Reified -> Aast.Reified

let hint_fun_decl ~params ~ret env =
  let ret_decl_ty =
    Decl_fun_utils.hint_to_type_opt env.decl_env (hint_of_type_hint ret)
  in
  let params_decl_ty =
    List.map
      ~f:(fun h ->
        Decl_fun_utils.hint_to_type_opt
          env.decl_env
          (hint_of_type_hint h.param_type_hint))
      params
  in
  (ret_decl_ty, params_decl_ty)

module Prov = struct
  let update ty ~f ~env =
    if TypecheckerOptions.using_extended_reasons env.genv.tcopt then
      map_reason ty ~f
    else
      ty

  let update_cty cty ~f ~env =
    if TypecheckerOptions.using_extended_reasons env.genv.tcopt then
      let (r, cstr) = deref_constraint_type cty in
      mk_constraint_type (f r, cstr)
    else
      cty

  let update_ity ity ~f ~env =
    match ity with
    | LoclType lty -> LoclType (update lty ~f ~env)
    | ConstraintType cty -> ConstraintType (update_cty cty ~f ~env)

  let flow ~from ~into = Typing_reason.flow (from, into)

  let rev r = Typing_reason.rev r

  let prj_fn_arg r ~idx_sub ~idx_super ~var =
    Typing_reason.(prj_symm (Prj_symm_fn_arg (idx_sub, idx_super, var), r))

  let prj_fn_ret r = Typing_reason.(prj_symm (Prj_symm_fn_ret, r))

  let prj_union_left r = Typing_reason.(prj_asymm_left (Prj_asymm_union, r))

  let prj_union_right r = Typing_reason.(prj_asymm_right (Prj_asymm_union, r))

  let prj_intersection_left r =
    Typing_reason.(prj_asymm_left (Prj_asymm_inter, r))

  let prj_intersection_right r =
    Typing_reason.(prj_asymm_right (Prj_asymm_inter, r))

  let prj_class r ~nm ~idx ~var =
    Typing_reason.prj_symm (Typing_reason.Prj_symm_class (nm, idx, var), r)

  let prj_newtype r ~nm ~idx ~var =
    Typing_reason.prj_symm (Typing_reason.Prj_symm_newtype (nm, idx, var), r)

  let prj_shape r ~fld_nm ~fld_kind_sub ~fld_kind_super =
    Typing_reason.(
      prj_symm (Prj_symm_shape (fld_nm, fld_kind_sub, fld_kind_super), r))
end

(** [refine_and_simplify_intersection ~hint_first env p reason ivar_pos ty hint_ty]
  intersects [ty] and [hint_ty], possibly making [hint_ty] support dynamic
  first if [ty] also supports dynamic.
  Then if the result has some like types in which prevent simplification
  of the intersection, it'll distribute any '&' at the top of the type tree
  in order to have a union at hand.
  This is because the resulting type will likely end up on the LHS of a subtyping
  call down the line, and unions on the LHS behave better than intersections,
  which result in incomplete typing.

  Parameters:
  * [reason]          The reason for the result types and other intermediate types.
  * [is_class]        Whether [hint_ty] is a class
  *)
let refine_and_simplify_intersection ~hint_first env ~is_class reason ty hint_ty
    =
  let intersect ~hint_first ~is_class env r ty hint_ty =
    let (env, hint_ty) =
      if
        is_class
        && TypecheckerOptions.enable_sound_dynamic (Env.get_tcopt env)
        && Utils.is_supportdyn env ty
      then
        Utils.make_supportdyn reason env hint_ty
      else
        (env, hint_ty)
    in
    (* Sometimes the type checker is sensitive to the ordering of intersections *)
    if hint_first then
      Inter.intersect env ~r hint_ty ty
    else
      Inter.intersect env ~r ty hint_ty
  in
  let like_type_simplify env ty hint_ty ~is_class =
    (* This basically distributes the intersection over the union.
       Let's call X the type to refine (`ty` here) and H the hint type.
       We want to intersect ~X with either H if H is enforced, or
       ~H if H is not enforced.

       If H is enforced: ~X & H is an intersection so may not behave well
       if passed as LHS of subtyping, so we distribute the intersection:

         ~X & H = (~ & H) | (X & H)

       (With the hope that (X & H) simplifies)


       If X is not enforced:

         ~X & ~H = ~(X & H)
    *)
    let (env, intersection_ty) =
      intersect ~hint_first:false ~is_class env reason ty hint_ty
    in
    let rec is_enforced hint_ty =
      match get_node hint_ty with
      | Toption ty -> is_nonnull ty || is_enforced ty
      | Tclass (_, _, [])
      | Tprim _ ->
        true
      | Tshape { s_fields = expected_fdm; _ } ->
        TShapeMap.for_all (fun _name ty -> is_enforced ty.sft_ty) expected_fdm
      | _ -> false
    in
    if is_enforced hint_ty then
      let (env, dyn_ty) =
        Inter.intersect env ~r:reason (Typing_make_type.dynamic reason) hint_ty
      in
      Union.union env dyn_ty intersection_ty
    else
      (env, MakeType.locl_like reason intersection_ty)
  in
  match Utils.try_strip_dynamic env ty with
  | Some ty when TypecheckerOptions.enable_sound_dynamic (Env.get_tcopt env) ->
    like_type_simplify env ty hint_ty ~is_class
  | _ -> intersect ~hint_first ~is_class env reason ty hint_ty
