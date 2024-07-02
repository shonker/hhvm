(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

type t = {
  implicit_inherit_sdt: bool;
  everything_sdt: bool;
  enable_strict_const_semantics: int;
}
[@@deriving show]

let from_global_options (gopt : GlobalOptions.t) =
  let open GlobalOptions in
  {
    implicit_inherit_sdt = gopt.tco_implicit_inherit_sdt;
    everything_sdt = gopt.po.ParserOptions.everything_sdt;
    enable_strict_const_semantics = gopt.tco_enable_strict_const_semantics;
  }
