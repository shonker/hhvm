(*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

type warn = private Warn_tag

type migrated = private Migrated_tag

type quickfix = {
  can_be_captured: bool;
  original_pos: Pos.t;
  replacement_pos: Pos.t;
}

module IsAsAlways = struct
  type kind =
    | Is_is_always_true
    | Is_is_always_false
    | As_always_succeeds of quickfix
    | As_always_fails

  type t = {
    kind: kind;
    lhs_ty: string;
    rhs_ty: string;
  }
end

module SketchyNullCheck = struct
  type kind =
    | Coalesce
    | Eq
    | Neq

  type t = {
    name: string option;
    kind: kind;
  }
end

module NonDisjointCheck = struct
  type t = {
    name: string;
    ty1: string;
    ty2: string;
    dynamic: bool;
  }
end

module SketchyEquality = struct
  type t = {
    result: bool;
    left: Pos_or_decl.t Message.t list Lazy.t;
    right: Pos_or_decl.t Message.t list Lazy.t;
    left_trail: Pos_or_decl.t list;
    right_trail: Pos_or_decl.t list;
  }
end

type (_, _) kind =
  | Sketchy_equality : (SketchyEquality.t, warn) kind
  | Is_as_always : (IsAsAlways.t, migrated) kind
  | Sketchy_null_check : (SketchyNullCheck.t, migrated) kind
  | Non_disjoint_check : (NonDisjointCheck.t, migrated) kind

type ('x, 'a) t = Pos.t * ('x, 'a) kind * 'x
