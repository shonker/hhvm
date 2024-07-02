(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

type pos_id = Pos_or_decl.t * Ast_defs.id_ [@@deriving eq, hash, ord, show]

type arg_position =
  | Aonly
  | Afirst
  | Asecond
[@@deriving eq, show]

type expr_dep_type_reason =
  | ERexpr of Expression_id.t
  | ERstatic
  | ERclass of string
  | ERparent of string
  | ERself of string
  | ERpu of string
[@@deriving eq, show]

type blame_source =
  | BScall
  | BSlambda
  | BSassignment
  | BSout_of_scope
[@@deriving eq, show]

type blame = Blame of Pos.t * blame_source [@@deriving eq, show]

(* All the possible types, reason is a trace of why a type
   was inferred in a certain way.

   Types exists in two phases. Phase one is 'decl_phase', meaning it is a type that
   was declared in user code. Phase two is 'locl_phase', meaning it is a type that is
   inferred via local inference.
*)
(* create private types to represent the different type phases *)
type decl_phase = private DeclPhase [@@deriving eq, hash, show]

type locl_phase = private LoclPhase [@@deriving eq, hash, show]

(* The phase below helps enforce that only Pos_or_decl.t positions end up in the heap.
 * To enforce that, any reason taking a Pos.t should be a locl_phase t_
 * to prevent a decl ty from using it.
 * Reasons used for decl types should be 'phase t_ so that they can be localized
 * to be used in the localized version of the type. *)

type field_kind =
  | Absent
  | Optional
  | Required
[@@deriving hash]

(** A symmetric projection into a type constructor *)
type prj_symm =
  | Prj_symm_neg
  | Prj_symm_class of string * int * Ast_defs.variance
  | Prj_symm_newtype of string * int * Ast_defs.variance
  | Prj_symm_tuple of int
  | Prj_symm_shape of string * field_kind * field_kind
  | Prj_symm_fn_arg of int * int * Ast_defs.variance
  | Prj_symm_fn_ret
  | Prj_symm_access
[@@deriving hash]

type prj_asymm =
  | Prj_asymm_union
  | Prj_asymm_inter
  | Prj_asymm_neg
[@@deriving hash]

(** The reason why something is expected to have a certain type *)
type _ t_ [@@deriving hash, show]

type t = locl_phase t_ [@@deriving show]

val to_json : 'a t_ -> Hh_json.json

val debug : t -> (Pos_or_decl.t * string) list

val explain : t -> complexity:int -> (Pos_or_decl.t * string) list

type decl_t = decl_phase t_

val localize : decl_t -> t

(** Translate a reason to a (pos, string) list, suitable for error_l. This
    previously returned a string, however the need to return multiple lines with
    multiple locations meant that it needed to more than convert to a string *)
val to_string : string -> 'phase t_ -> (Pos_or_decl.t * string) list

val to_pos : 'phase t_ -> Pos_or_decl.t

val to_constructor_string : 'phase t_ -> string

type ureason =
  | URnone
  | URassign
  | URassign_inout
  | URhint
  | URreturn
  | URforeach
  | URthrow
  | URvector
  | URkey of string
  | URvalue of string
  | URawait
  | URyield
  | URxhp of string * string  (** Name of XHP class, Name of XHP attribute *)
  | URxhp_spread
  | URindex of string
  | URelement of string
  | URparam
  | URparam_inout
  | URarray_value
  | URpair_value
  | URtuple_access
  | URpair_access
  | URnewtype_cstr
  | URclass_req
  | URenum
  | URenum_include
  | URenum_cstr
  | URenum_underlying
  | URenum_incompatible_cstr
  | URtypeconst_cstr
  | URsubsume_tconst_cstr
  | URsubsume_tconst_assign
  | URclone
  | URusing
  | URstr_concat
  | URstr_interp
  | URdynamic_prop
  | URlabel
[@@deriving show]

val index_array : ureason

val index_tuple : ureason

val index_class : string -> ureason

val set_element : string -> ureason

val string_of_ureason : ureason -> string

val none : 'phase t_

val witness : Pos.t -> t

val witness_from_decl : Pos_or_decl.t -> 'phase t_

(* Used as an index into a vector-like
   array or string. Position of indexing,
   reason for the indexed type *)
val idx : Pos.t * t -> t

(* Used as an index, in the Vector case *)
val idx_vector : Pos.t -> t

val idx_vector_from_decl : Pos_or_decl.t -> 'phase t_

(* Because it is iterated in a foreach loop *)
val foreach : Pos.t -> t

(* Because it is iterated "await as" in foreach *)
val asyncforeach : Pos.t -> t

val arith : Pos.t -> t

val arith_ret : Pos.t -> t

(* pos, arg float typing reason, arg position *)
val arith_ret_float : Pos.t * t * arg_position -> t

(* pos, arg num typing reason, arg position *)
val arith_ret_num : Pos.t * t * arg_position -> t

val arith_ret_int : Pos.t -> t

val arith_dynamic : Pos.t -> t

val bitwise_dynamic : Pos.t -> t

val incdec_dynamic : Pos.t -> t

val comp : Pos.t -> t

val concat_ret : Pos.t -> t

val logic_ret : Pos.t -> t

val bitwise : Pos.t -> t

val bitwise_ret : Pos.t -> t

val no_return : Pos.t -> t

val no_return_async : Pos.t -> t

val ret_fun_kind : Pos.t * Ast_defs.fun_kind -> t

val ret_fun_kind_from_decl : Pos_or_decl.t * Ast_defs.fun_kind -> 'phase t_

val hint : Pos_or_decl.t -> 'phase t_

val throw : Pos.t -> t

val placeholder : Pos.t -> t

val ret_div : Pos.t -> t

val yield_gen : Pos.t -> t

val yield_asyncgen : Pos.t -> t

val yield_asyncnull : Pos.t -> t

val yield_send : Pos.t -> t

val lost_info : string * t * blame -> t

val format : Pos.t * string * t -> t

val class_class : Pos_or_decl.t * string -> 'phase t_

val unknown_class : Pos.t -> t

val var_param : Pos.t -> t

val var_param_from_decl : Pos_or_decl.t -> 'phase t_

(* splat pos, fun def pos, number of args before splat *)
val unpack_param : Pos.t * Pos_or_decl.t * int -> t

val inout_param : Pos_or_decl.t -> 'phase t_

val instantiate : 'phase t_ * string * 'phase t_ -> 'phase t_

val typeconst :
  'phase t_ * (Pos_or_decl.t * string) * string Lazy.t * 'phase t_ -> 'phase t_

val type_access : t * (t * string Lazy.t) list -> t

val expr_dep_type :
  'phase t_ * Pos_or_decl.t * expr_dep_type_reason -> 'phase t_

(* ?-> operator is used *)
val nullsafe_op : Pos.t -> t

val tconst_no_cstr : pos_id -> 'phase t_

val predicated : Pos.t * string -> t

val is_refinement : Pos.t -> t

val as_refinement : Pos.t -> t

val equal : Pos.t -> t

val varray_or_darray_key : Pos_or_decl.t -> 'phase t_

val vec_or_dict_key : Pos_or_decl.t -> 'phase t_

val using : Pos.t -> t

val dynamic_prop : Pos.t -> t

val dynamic_call : Pos.t -> t

val dynamic_construct : Pos.t -> t

val idx_dict : Pos.t -> t

val idx_set_element : Pos.t -> t

val missing_optional_field : Pos_or_decl.t * string -> 'phase t_

val unset_field : Pos.t * string -> t

val contravariant_generic : t * string -> t

val invariant_generic : t * string -> t

val regex : Pos.t -> t

val implicit_upper_bound : Pos_or_decl.t * string -> 'phase t_

val type_variable : Pos.t -> t

val type_variable_generics : Pos.t * string * string -> t

val type_variable_error : Pos.t -> t

val global_type_variable_generics : Pos_or_decl.t * string * string -> 'phase t_

val solve_fail : Pos_or_decl.t -> 'phase t_

val cstr_on_generics : Pos_or_decl.t * pos_id -> 'phase t_

val lambda_param : Pos.t * t -> t

val shape : Pos.t * string -> t

val shape_literal : Pos.t -> t

val enforceable : Pos_or_decl.t -> 'phase t_

val destructure : Pos.t -> t

val key_value_collection_key : Pos.t -> t

val global_class_prop : Pos_or_decl.t -> 'phase t_

val global_fun_param : Pos_or_decl.t -> 'phase t_

val global_fun_ret : Pos_or_decl.t -> 'phase t_

val splice : Pos.t -> t

val et_boolean : Pos.t -> t

val default_capability : Pos_or_decl.t -> 'phase t_

val concat_operand : Pos.t -> t

val interp_operand : Pos.t -> t

val dynamic_coercion : t -> t

val support_dynamic_type : Pos_or_decl.t -> 'phase t_

val dynamic_partial_enforcement : Pos_or_decl.t * string * t -> t

val rigid_tvar_escape : Pos.t * string * string * t -> t

val opaque_type_from_module : Pos_or_decl.t * string * t -> t

val missing_class : Pos.t -> t

val invalid : 'phase t_

val captured_like : Pos.t -> t

val pessimised_inout : Pos_or_decl.t -> 'phase t_

val pessimised_return : Pos_or_decl.t -> 'phase t_

val pessimised_prop : Pos_or_decl.t -> 'phase t_

val unsafe_cast : Pos.t -> t

val pattern : Pos.t -> t

(* Reasons record a linear path through the program, so we have a constructor
   to concatenate two paths *)
val flow : t * t -> t

(* Paths are reversed with contravariance; we use this constructor to perform reversals lazily *)
val rev : t -> t

(* Records reasons through type constructors where both sub- and supertype are projected in the same way *)
val prj_symm : prj_symm * t -> t

val prj_asymm_left : prj_asymm * t -> t

(* Records reasons through type constructors where only one of the sub- or supertype is projected *)
val prj_asymm_right : prj_asymm * t -> t

val missing_field : t

val pessimised_this : Pos_or_decl.t -> 'phase t_

val compare : 'phase t_ -> 'phase t_ -> int

val map_pos :
  (Pos.t -> Pos.t) -> (Pos_or_decl.t -> Pos_or_decl.t) -> 'phase t_ -> 'phase t_

val force_lazy_values : t -> t

module Predicates : sig
  val is_none : t -> bool

  val is_opaque_type_from_module : t -> bool

  val is_instantiate : t -> bool

  val is_hint : t -> bool

  val unpack_expr_dep_type_opt :
    'phase t_ -> ('phase t_ * Pos_or_decl.t * expr_dep_type_reason) option

  val unpack_unpack_param_opt : t -> (Pos.t * Pos_or_decl.t * int) option

  val unpack_cstr_on_generics_opt : 'phase t_ -> (Pos_or_decl.t * pos_id) option

  val unpack_shape_literal_opt : t -> Pos.t option
end
