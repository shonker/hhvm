(*
 * Copyright (c) 2015, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

open Hh_prelude

type mode =
  | ForTypecheck
  | ForCodegen
[@@deriving eq, hash, show, ord]

type env = {
  ns_ns_uses: string SMap.t; [@opaque]
  ns_class_uses: string SMap.t; [@opaque]
  ns_fun_uses: string SMap.t; [@opaque]
  ns_const_uses: string SMap.t; [@opaque]
  ns_name: string option;
  ns_mode: mode;
  ns_disable_xhp_element_mangling: bool;
}
[@@deriving eq, hash, show, ord]

let hh_autoimport_map_of_list ids =
  List.map ids ~f:(fun id -> (id, "HH\\" ^ id)) |> SMap.of_list

let default_class_uses = hh_autoimport_map_of_list Hh_autoimport.types

let default_fun_uses = hh_autoimport_map_of_list Hh_autoimport.funcs

let default_const_uses = hh_autoimport_map_of_list Hh_autoimport.consts

let default_ns_uses = hh_autoimport_map_of_list Hh_autoimport.namespaces

let empty_with_default : env =
  let open ParserOptions in
  let popt = default in
  let auto_ns_map = popt.auto_namespace_map in
  let ns_mode =
    if popt.codegen then
      ForCodegen
    else
      ForTypecheck
  in
  let ns_disable_xhp_element_mangling = popt.disable_xhp_element_mangling in
  {
    ns_ns_uses = SMap.union (SMap.of_list auto_ns_map) default_ns_uses;
    ns_class_uses = default_class_uses;
    ns_fun_uses = default_fun_uses;
    ns_const_uses = default_const_uses;
    ns_name = None;
    ns_mode;
    ns_disable_xhp_element_mangling;
  }
