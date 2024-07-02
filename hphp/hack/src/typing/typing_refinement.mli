(*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

(** Represents a type in disjunctive normal form. For example
   the type (A & B) | C is represented as [[A; B]; [C]] *)
type dnf_ty = Typing_defs.locl_ty list list

(** The partition of type over [predicate].

  [left] describes a type that is a subset of the values that
    pass the predicate

  [right] describes a type that is a subset of the values that
    fail the predicate

  [span] describes a type whose set of values contain values that
    both pass and fail the predicate
*)
type ty_partition = {
  predicate: Typing_defs.type_predicate;
  left: dnf_ty;
  span: dnf_ty;
  right: dnf_ty;
}

(**
  Given a [Typing_defs.locl_ty] and a [Typing_defs.type_predicate], partition the
  [Typing_defs.locl_ty] into three types:

    left: For the portition of the type that is a subset of the values that
      passes the predicate at runtime

    right: For the portition of the type that is a subset of the values that
      fails the predicate at runtime

    span: For the portition of the type that contains values that both
      pass and fails the predicate

  As an example consider partition the type (num | arraykey | T) where T is a generic type
    bounded by nonnull over the predicate IsInt

  This will produce
    left = int (num & IsInt = int and arraykey & IsInt = int)
    right = float | string (num & !IsInt = float and arraykey & !IsInt = string)
    span = T (T can contain values that are ints and values that are not ints)
*)
val partition_ty :
  Typing_env_types.env ->
  Typing_defs.locl_ty ->
  Typing_defs.type_predicate ->
  ty_partition

module TyPredicate : sig
  val of_ty :
    Typing_env_types.env ->
    Typing_defs.locl_ty ->
    (Typing_env_types.env * Typing_defs.type_predicate) option

  val to_ty :
    Typing_defs.Reason.t -> Typing_defs.type_predicate -> Typing_defs.locl_ty
end
