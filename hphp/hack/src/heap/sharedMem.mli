(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

(** This is just a sentinel for self-documenting purposes which some
    parts of the codebase use. They take a parameter "uses_sharedmem : SharedMem.uses"
    as a way to indicate to their callers that they read/write sharedmem. *)
type uses = Uses

exception Out_of_shared_memory

exception Hash_table_full

exception Heap_full

exception Sql_assertion_failure of int

exception C_assertion_failure of string

(** Configuration object that initializes shared memory. *)
type config = {
  global_size: int;
  heap_size: int;
  dep_table_pow: int;
  hash_table_pow: int;
  shm_use_sharded_hashtbl: bool;
  shm_dirs: string list;
  shm_min_avail: int;
  log_level: int;
  sample_rate: float;
  (* 0 - lz4, others -- compression level for zstd*)
  compression: int;
}
[@@deriving show]

(** Default configuration object *)
val default_config : config

(** Empty configuration object *)
val empty_config : config

(** A handle to initialized shared memory. Used to connect other workers to
    shared memory.

    NOTE: If you change the order, update hh_shared.c! *)
type handle = private {
  h_fd: Unix.file_descr;
  h_global_size: int;
  h_heap_size: int;
  h_dep_table_pow_val: int;
  h_hash_table_pow_val: int;
  h_num_workers_val: int;
  h_shm_use_sharded_hashtbl: bool;
}

(** Initialize shared memory. *)
val init : config -> num_workers:int -> handle

(** Connect other workers to shared memory *)
val connect : handle -> worker_id:int -> unit

(** Get the handle to shared memory. Returns nonsense if the current
    process hasn't yet connected to shared memory *)
val get_handle : unit -> handle

(** Allow or disallow remove operations. *)
val set_allow_removes : bool -> unit

(** Allow or disallow shared memory writes for the current process. *)
val set_allow_hashtable_writes_by_current_process : bool -> unit

(** Invoke the garbage collector. *)
val hh_collect : unit -> unit

(** Directly access the shared memory table.

    This can be used to provide proxying across the network *)
module RawAccess : sig
  type serialized = private bytes

  val mem_raw : string -> bool

  val get_raw : string -> serialized

  val add_raw : string -> serialized -> unit

  val deserialize_raw : serialized -> 'a

  val serialize_raw : 'a -> serialized
end

(** Access the legacy dependency graph *)
module DepTable : sig
  (** Get the number of in-memory dep table entries *)
  val get_in_memory_dep_table_entry_count : unit -> int

  (** Return the filename of the loaded SQLite dep table. Returns None if no
      SQLite dep table was loaded. *)
  val loaded_dep_table_filename : unit -> string option

  (** Save the dep table to an SQLite database.

      Only supports a full init. Will raise an exception if called when
      hh_server was launched with a saved state.

      Returns the number of edges written to the SQLite databse. *)
  val save_dep_table_sqlite : fn:string -> build_revision:string -> int

  (** Save the dep table to a binary blob.

      Will raise an exception if called when hh_server was launched with a
      saved state. This function is only used to support remote workers,
      which have to communicate discovered edges back to the master process.

      If [reset_state_after_saving] is set to true, the in-memory dep table
      will be empied after this operation.

      Returns the number of edges written to the blob.  *)
  val save_dep_table_blob :
    fn:string -> build_revision:string -> reset_state_after_saving:bool -> int

  (** Update the given SQLite database by adding each edge in the dep table
      to it.

      Returns the number of edges written to the SQLite databse. *)
  val update_dep_table_sqlite : fn:string -> build_revision:string -> int

  (** Attach the given SQLite dep table. *)
  val load_dep_table_sqlite : fn:string -> ignore_hh_version:bool -> unit

  (** Load the edges in the given binary blob into the in-memory dep table. *)
  val load_dep_table_blob : fn:string -> ignore_hh_version:bool -> int

  (** Clean up the artifacts generated by SQLite. *)
  val cleanup_sqlite : unit -> unit
end

(** Some telemetry utilities *)
module SMTelemetry : sig
  (** Get some shared-memory telemetry. Even works when shared memory hasn't
    been initialized yet. *)
  val get_telemetry : unit -> Telemetry.t

  (** Return the number of bytes allocated in shared memory. This includes
      bytes that were free'd but are not yet available for reuse. *)
  val heap_size : unit -> int

  (** Returns the number of bytes not reachable fro hashtable entries. *)
  val wasted_heap_size : unit -> int

  (** The logging level for shared memory statistics:
        - 0 = nothing
        - 1 = log totals, averages, min, max bytes marshalled and unmarshalled
    *)
  val hh_log_level : unit -> int

  (** Get the sample rate for shared memory statistics. *)
  val hh_sample_rate : unit -> float

  (** Get the number of used slots in our hashtable. *)
  val hash_used_slots : unit -> int * int

  (** Get the number of total slots in our hashtable. *)
  val hash_slots : unit -> int

  (** Get the number of used slots in our dependency table. *)
  val dep_used_slots : unit -> int

  (** Get the total number of slots in our dependency table. *)
  val dep_slots : unit -> int

  type table_stats = {
    nonempty_slots: int;
    used_slots: int;
    slots: int;
  }

  (** Combine [dep_used_slots] and [dep_slots] *)
  val dep_stats : unit -> table_stats

  (** Combine [hash_used_slots] and [hash_slots] *)
  val hash_stats : unit -> table_stats

  (** Not sure. Return the removed number of entries? *)
  val hh_removed_count : unit -> int

  (** Did we overflow the heap? *)
  val is_heap_overflow : unit -> bool

  (** Compute the size of values in the garbage-collected heap. (???) *)
  val value_size : Obj.t -> int

  (** Log to our telemetry infra that we successfully initialized shared
      memory *)
  val init_done : unit -> unit
end

(** Interface to the garbage collector *)
module GC : sig
  val should_collect : [ `aggressive | `always_TEST | `gentle ] -> bool

  val collect : [ `aggressive | `always_TEST | `gentle ] -> unit
end

(** Key in the big shared-memory table.

    Each key is built by concatenating an optional "old" prefix, a heap-prefix
    and an object-specific key.

    The key module provides a way to convert the result of this string
    concatenation to an MD5-hash, of which the first few bytes are used
    to index the hashtable. *)
module type Key = sig
  (** The type of keys that OCaml-land callers try to insert.

      This key will be object-specific (unique within a heap), but might not be
      unique across heaps. *)
  type userkey

  (** The concatenation of the heap-prefix with the object-specific [userkey].

     This key is unique across all heaps. *)
  type t

  (** The normal [t] key prefixed with the old-prefix.

     This is used to distinguish between new and old values of the same
     object in the same heap. *)
  type old

  (** The md5 of an [old] or a new [key].

     We will use the first few bytes of this MD5 digest to index into
     the hash table. *)
  type md5

  val make : Prefix.t -> userkey -> t

  val make_old : Prefix.t -> userkey -> old

  val to_old : t -> old

  val new_from_old : old -> t

  val md5 : t -> md5

  val md5_old : old -> md5

  (** Note that this returns the raw MD5 bytes, not its hex encoding. *)
  val string_of_md5 : md5 -> string
end

(** The interface that all keys need to implement *)
module type UserKeyType = sig
  type t

  val to_string : t -> string

  val compare : t -> t -> int
end

(** Make a new key that can be stored in shared-memory. *)
module KeyFunctor (UserKeyType : UserKeyType) :
  Key with type userkey = UserKeyType.t

(** Heap type that represents immediate access to the underlying hashtable. *)
module type Raw = functor (Key : Key) (Value : Value.Type) -> sig
  val add : Key.md5 -> Value.t -> unit

  val mem : Key.md5 -> bool

  val get : Key.md5 -> Value.t

  val remove : Key.md5 -> unit

  val move : Key.md5 -> Key.md5 -> unit
end

(** Heap that provides immediate access to the underlying hashtable. *)
module Immediate : Raw

type 'a profiled_value =
  | RawValue of 'a
  | ProfiledValue of {
      entry: 'a;
      write_time: float;
    }

(** Heap that provides profiled access (?) to the underlying hashtable. *)
module ProfiledImmediate : Raw

(** Heap that provides direct access to shared memory, but with a layer
    of local changes that allows us to decide whether or not to commit
    specific values. *)
module WithLocalChanges (Raw : Raw) (Key : Key) (Value : Value.Type) : sig
  include module type of Raw (Key) (Value)

  module LocalChanges : sig
    val has_local_changes : unit -> bool

    val push_stack : unit -> unit

    val pop_stack : unit -> unit

    val revert : Key.md5 -> unit

    val commit : Key.md5 -> unit

    val revert_all : unit -> unit

    val commit_all : unit -> unit
  end
end

(** Heap used to access "new" values (as opposed to "old" ones).

    There are several cases where we need to compare the old and the new
    representations of objects to determine what has changed.

    The "old" representation is the value that was bound to that key in
    the last round of type-checking. *)
module New (Raw : Raw) (Key : Key) (Value : Value.Type) : sig
  (** Adds a binding to the table.

      Note: the table is left unchanged if the key was already bound!
      TODO(hverr): this will no longer be true with sharded hash tables. *)
  val add : Key.t -> Value.t -> unit

  val get : Key.t -> Value.t option

  val find_unsafe : Key.t -> Value.t (* may throw {!Shared_mem_not_found} *)

  val remove : Key.t -> unit

  val mem : Key.t -> bool

  (** Marks the value bound to this key as "old".

      In pratice, the value will be moved to the same key, prefixed with
      the old-prefix. *)
  val oldify : Key.t -> unit

  module WithLocalChanges : module type of WithLocalChanges (Raw) (Key) (Value)
end

(** Same as [New] but for old values. *)
module Old
    (Raw : Raw)
    (Key : Key)
    (Value : Value.Type)
    (_ : module type of WithLocalChanges (Raw) (Key) (Value)) : sig
  val get : Key.old -> Value.t option

  val remove : Key.old -> unit

  val mem : Key.old -> bool

  (** Takes an old value and moves it back to a "new" one *)
  val revive : Key.old -> unit
end

(** A heap for a user-defined type. *)
module type NoCache = sig
  type key

  type t

  module KeySet : Set.S with type elt = key

  module KeyMap : WrappedMap.S with type key = key

  val add : key -> t -> unit

  val get : key -> t option

  val get_old : key -> t option

  val get_old_batch : KeySet.t -> t option KeyMap.t

  val remove_old_batch : KeySet.t -> unit

  val find_unsafe : key -> t (* May throw {!Shared_mem_not_found} *)

  val get_batch : KeySet.t -> t option KeyMap.t

  val remove_batch : KeySet.t -> unit

  val mem : key -> bool

  val mem_old : key -> bool

  val oldify_batch : KeySet.t -> unit

  val revive_batch : KeySet.t -> unit

  module LocalChanges : sig
    val has_local_changes : unit -> bool

    val push_stack : unit -> unit

    val pop_stack : unit -> unit

    val revert_batch : KeySet.t -> unit

    val commit_batch : KeySet.t -> unit

    val revert_all : unit -> unit

    val commit_all : unit -> unit
  end
end

(** A heap for a user-defined type.

    Provides no worker-local caching. Directly stores to and queries from
    shared memory. *)
module NoCache (_ : Raw) (UserKeyType : UserKeyType) (Value : Value.Type) :
  NoCache
    with type key = UserKeyType.t
     and type t = Value.t
     and module KeySet = Set.Make(UserKeyType)
     and module KeyMap = WrappedMap.Make(UserKeyType)

(** A process-local cache type. *)
module type CacheType = sig
  type key

  type value

  val add : key -> value -> unit

  val get : key -> value option

  val remove : key -> unit

  val clear : unit -> unit

  val get_size : unit -> int

  val get_telemetry_items_and_keys : unit -> Obj.t * key Seq.t
end

module type LocalHashtblConfigType = sig
  (* The type of object we want to keep in cache *)
  type value

  (* The capacity of the cache *)
  val capacity : int
end

module FreqCache (Key : sig
  type t
end)
(LocalHashtblConfig : LocalHashtblConfigType) :
  CacheType with type key := Key.t and type value := LocalHashtblConfig.value

module OrderedCache (Key : sig
  type t
end)
(LocalHashtblConfig : LocalHashtblConfigType) :
  CacheType with type key := Key.t and type value := LocalHashtblConfig.value

module type LocalCapacityType = sig
  val capacity : int
end

(** Invalidate all worker-local caches *)
val invalidate_local_caches : unit -> unit

module type LocalCache = sig
  type key

  type value

  module L1 : CacheType with type key = key and type value = value

  module L2 : CacheType with type key = key and type value = value

  val add : key -> value -> unit

  val get : key -> value option

  val remove : key -> unit

  val clear : unit -> unit

  val get_telemetry : Telemetry.t -> Telemetry.t
end

module LocalCache
    (UserKeyType : UserKeyType)
    (Value : Value.Type)
    (_ : LocalCapacityType) :
  LocalCache with type key = UserKeyType.t and type value = Value.t

(** Same as [NoCache] but provides a worker-local cache. *)
module type WithCache = sig
  include NoCache

  val write_around : key -> t -> unit

  val get_no_cache : key -> t option

  module Cache : LocalCache with type key = key and type value = t
end

module WithCache
    (_ : Raw)
    (UserKeyType : UserKeyType)
    (Value : Value.Type)
    (_ : LocalCapacityType) :
  WithCache
    with type key = UserKeyType.t
     and type t = Value.t
     and module KeySet = Set.Make(UserKeyType)
     and module KeyMap = WrappedMap.Make(UserKeyType)
