(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

module LMap = Local_id.Map

type mode =
  | FlagTargets
      (** Flag all possible targets, e.g., `dict['k1' => 42, 'k2' =>
          'meaning']` without performing any analysis *)
  | DumpConstraints  (** Dump constraints generated by analysing the program *)
  | SimplifyConstraints
      (** Partially solve key constraints within functions and methods and
          report back summaries about which `dict`s might be `shape`s and which
          functions/methods they depend on. *)
  | SolveConstraints
      (** Globally solve the key constraints and report back `dict`s that can
          be `shape`s along with the `shape` keys *)

type options = { mode: mode }

type entity_ = Literal of Pos.t

type entity = entity_ option

type shape_key = SK_string of string [@@deriving eq, ord]

type constraint_ =
  | Exists of entity_  (** Records existence of a dict *)
  | Has_static_key of entity_ * shape_key * Typing_defs.locl_ty
      (** Records the static key an entity is accessed with along with the Hack
          type of the key *)
  | Has_dynamic_key of entity_
      (** Records that an entity is accessed with a dynamic key *)

type shape_result =
  | Shape_result of entity_ * (shape_key * Typing_defs.locl_ty) list
      (** A particular dict that acts like a shape along with its keys and
          types the keys point to *)

type env = {
  constraints: constraint_ list;  (** Append-only set of constraints *)
  lenv: entity LMap.t;  (** Local variable information *)
  saved_env: Tast.saved_env;
      (** Environment stored in the TAST used to expand types *)
}
