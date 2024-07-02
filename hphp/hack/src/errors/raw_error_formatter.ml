(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

open Hh_prelude
open String_utils

let format_msg file_clr err_clr ((p, s) : Pos.absolute * string) =
  let (line, start, end_) = Pos.info_pos p in
  let file_path =
    if Unix.isatty Unix.stdout then
      let cwd = Filename.concat (Sys.getcwd ()) "" in
      lstrip (Pos.filename p) cwd
    else
      Pos.filename p
  in
  let line_clr = Tty.Normal Tty.Yellow in
  let col_clr = Tty.Normal Tty.Cyan in
  let default_clr = Tty.Normal Tty.Default in
  [
    (file_clr, file_path);
    (default_clr, ":");
    (line_clr, string_of_int line);
    (default_clr, ":");
    (col_clr, string_of_int start);
    (default_clr, ",");
    (col_clr, string_of_int end_);
    (default_clr, ": ");
    (err_clr, s);
  ]

let format_error_code code =
  [
    (Tty.Normal Tty.Default, " (");
    (Tty.Normal Tty.Yellow, User_error.error_code_to_string code);
    (Tty.Normal Tty.Default, ")");
  ]

let format_severity (severity : User_error.severity) =
  [
    ( Tty.Bold (User_error.Severity.tty_color severity),
      Printf.sprintf "%s: " (User_error.Severity.to_all_caps_string severity) );
  ]

let to_string (error : Errors.finalized_error) : string =
  let {
    User_error.severity;
    code;
    claim;
    reasons;
    quickfixes = _;
    custom_msgs = _;
    is_fixmed = _;
    flags = _;
  } =
    error
  in
  let newline = (Tty.Normal Tty.Default, "\n") in
  let severity = format_severity severity in
  let claim =
    format_msg (Tty.Bold Tty.Red) (Tty.Bold Tty.Red) claim
    @ format_error_code code
    @ [newline]
  in
  let reasons =
    let indent = (Tty.Normal Tty.Default, "  ") in
    List.concat_map
      ~f:(fun msg ->
        (indent :: format_msg (Tty.Normal Tty.Red) (Tty.Normal Tty.Green) msg)
        @ [newline])
      reasons
  in
  let custom_msgs =
    match error.User_error.custom_msgs with
    | [] -> []
    | msgs ->
      (Tty.Normal Tty.Default, "\n")
      :: List.map ~f:(fun msg -> (Tty.Normal Tty.Yellow, msg)) msgs
  in
  let to_print = severity @ claim @ reasons @ custom_msgs in
  if Unix.isatty Unix.stdout then
    List.map to_print ~f:(fun (c, s) -> Tty.apply_color c s) |> String.concat
  else
    List.map to_print ~f:(fun (_, x) -> x) |> String.concat
