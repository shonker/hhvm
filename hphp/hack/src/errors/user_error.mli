(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

type severity =
  | Err
  | Warning
[@@deriving ord]

module Severity : sig
  val to_string : severity -> string

  val to_all_caps_string : severity -> string

  val tty_color : severity -> Tty.raw_color
end

type ('prim_pos, 'pos) t = {
  severity: severity;
  code: int;
  claim: 'prim_pos Message.t;
  reasons: 'pos Message.t list;
  quickfixes: 'prim_pos Quickfix.t list; [@hash.ignore]
  custom_msgs: string list;
  is_fixmed: bool;
  flags: User_error_flags.t;
}
[@@deriving eq, hash, ord, show]

val make :
  severity ->
  int ->
  ?is_fixmed:bool ->
  ?quickfixes:'a Quickfix.t list ->
  ?custom_msgs:string list ->
  ?flags:User_error_flags.t ->
  'a Message.t ->
  'b Message.t list ->
  ('a, 'b) t

(** Make a User_error.t with severity Err *)
val make_err :
  int ->
  ?is_fixmed:bool ->
  ?quickfixes:'a Quickfix.t list ->
  ?custom_msgs:string list ->
  ?flags:User_error_flags.t ->
  'a Message.t ->
  'b Message.t list ->
  ('a, 'b) t

(** Make a User_error.t with severity Warning *)
val make_warning :
  int ->
  ?is_fixmed:bool ->
  ?quickfixes:'a Quickfix.t list ->
  ?custom_msgs:string list ->
  ?flags:User_error_flags.t ->
  'a Message.t ->
  'b Message.t list ->
  ('a, 'b) t

val get_code : ('a, 'b) t -> int

val get_pos : ('a, 'b) t -> 'a

val quickfixes : ('a, 'b) t -> 'a Quickfix.t list

val to_list : ('a, 'a) t -> 'a Message.t list

val to_list_ : (Pos.t, Pos_or_decl.t) t -> Pos_or_decl.t Message.t list

val get_messages : ('a, 'a) t -> 'a Message.t list

val to_absolute : (Pos.t, Pos_or_decl.t) t -> (Pos.absolute, Pos.absolute) t

val to_relative : (Pos.t, Pos_or_decl.t) t -> (Pos.t, Pos.t) t

val make_absolute : severity -> int -> 'a Message.t list -> ('a, 'a) t

val to_absolute_for_test :
  (Pos.t, Pos_or_decl.t) t -> (Pos.absolute, Pos.absolute) t

val error_kind : int -> string

val error_code_to_string : int -> string

(** The string looks like:

    ERROR/WARN: path/to/file.php pos...
    error/warning: You have a problem here (Typing4110)
      path/to/other/file.php pos...
      Because this is wrong
      path/to/other/file.php pos...
      And this too
  *)
val to_string : bool -> (Pos.absolute, Pos.absolute) t -> string

val to_json :
  human_formatter:(('a Pos.pos, 'a Pos.pos) t -> string) option ->
  filename_to_string:('a -> string) ->
  ('a Pos.pos, 'a Pos.pos) t ->
  Hh_json.json
