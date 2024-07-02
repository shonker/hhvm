(*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

module ErrorHash : sig
  type t = int
end

include Set.S with type elt = ErrorHash.t

val pp : Format.formatter -> t -> unit
