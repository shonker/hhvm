(*
 * Copyright (c) 2016, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *
 *)

open Hh_prelude
open Integration_test_base_types
open Reordered_argument_collections
open ServerCommandTypes
open Int.Replace_polymorphic_compare

exception Integration_test_failure

module FileMap = SMap
module ErrorSet = SSet

let root = "/"

let hhi = "/hhi"

let tmp = "/tmp"

let () = Folly.ensure_folly_init ()

let server_config = ServerEnvBuild.default_genv.ServerEnv.config

let po =
  ParserOptions.
    {
      default with
      disable_xhp_element_mangling = false;
      deregister_php_stdlib = true;
    }

let global_opts =
  GlobalOptions.set
    ~po
    ~tco_num_local_workers:1
    ~tco_saved_state:GlobalOptions.default_saved_state
    ~tco_fetch_remote_old_decls:false
    GlobalOptions.default

let server_config = ServerConfig.set_tc_options server_config global_opts

let server_config = ServerConfig.set_parser_options server_config po

let genv =
  ref { ServerEnvBuild.default_genv with ServerEnv.config = server_config }

let did_init = ref false

(* Init part common to fresh and saved state init *)
let test_init_common ?(hhi_files = []) () =
  if !did_init then
    failwith
      ("Initializing the server twice in same process. There is no guarantee of global state "
      ^ "cleanup between them. Split your test in multiple separate tests, or run individual test "
      ^ "cases in separate processes using Test.in_daemon.")
  else
    did_init := true;

  Unix.putenv "HH_TEST_MODE" "1";
  Printexc.record_backtrace true;
  EventLogger.init_fake ();
  Relative_path.set_path_prefix Relative_path.Root (Path.make root);
  Relative_path.set_path_prefix Relative_path.Hhi (Path.make hhi);
  Relative_path.set_path_prefix Relative_path.Tmp (Path.make tmp);
  Server_progress.disable ();

  let handle = SharedMem.init ~num_workers:0 SharedMem.default_config in
  ignore (handle : SharedMem.handle);

  ServerMain.force_break_recheck_loop_for_test true;

  List.iter hhi_files ~f:(fun (fn, contents) ->
      TestDisk.set (Filename.concat hhi fn) contents);
  ()

(* Hhi files are loaded during server setup. If given a list of string + contents, we add them
   to the test disk and add them to disk_needs_parsing. After one server run loop, they will be loaded.
   This isn't exactly the same as how initialization does it, but the purpose is not to test the hhi
   files, but to test incremental mode behavior with Hhi files present.
*)
let setup_server ?custom_config ?(hhi_files = []) ?edges_dir () : ServerEnv.env
    =
  test_init_common () ~hhi_files;

  let init_id = Random_id.short_string () in
  let deps_mode =
    match edges_dir with
    | Some edges_dir ->
      Typing_deps_mode.SaveToDiskMode
        {
          graph = None;
          new_edges_dir = edges_dir;
          human_readable_dep_map_dir = None;
        }
    | None -> Typing_deps_mode.InMemoryMode None
  in
  let env =
    match custom_config with
    | Some config -> ServerEnvBuild.make_env ~init_id ~deps_mode config
    | None -> ServerEnvBuild.make_env ~init_id ~deps_mode !genv.ServerEnv.config
  in
  let hhi_file_list =
    List.map hhi_files ~f:(fun (fn, _) ->
        Relative_path.create Relative_path.Hhi (Filename.concat hhi fn))
  in
  let hhi_set = Relative_path.Set.of_list hhi_file_list in

  Server_progress.disable ();
  (* Return environment *)
  { env with ServerEnv.disk_needs_parsing = hhi_set }

let default_loop_input = { disk_changes = []; new_client = None }

let run_loop_once :
    type a. ServerEnv.env -> a loop_inputs -> ServerEnv.env * a loop_outputs =
 fun env inputs ->
  TestClientProvider.clear ();
  Option.iter inputs.new_client ~f:(function RequestResponse x ->
      TestClientProvider.mock_new_client_type Non_persistent;
      TestClientProvider.mock_client_request x);

  let client_provider = ClientProvider.provider_for_test () in
  let disk_changes =
    List.map inputs.disk_changes ~f:(fun (x, y) -> (root ^ x, y))
  in
  List.iter disk_changes ~f:(fun (path, contents) -> TestDisk.set path contents);

  let did_read_disk_changes_ref = ref false in
  let get_changes_sync () =
    ServerNotifier.SyncChanges
      (if not !did_read_disk_changes_ref then (
        did_read_disk_changes_ref := true;
        SSet.of_list (List.map disk_changes ~f:fst)
      ) else
        SSet.empty)
  in
  let get_changes_async () = get_changes_sync () in
  let genv =
    {
      !genv with
      ServerEnv.notifier =
        ServerNotifier.init_mock ~get_changes_sync ~get_changes_async;
    }
  in
  (* Always pick up disk changes in tests immediately *)
  let env = ServerEnv.{ env with last_notifier_check_time = 0.0 } in
  let env = ServerMain.serve_one_iteration genv env client_provider in
  let {
    ServerEnv.RecheckLoopStats.total_changed_files_count;
    total_rechecked_count;
    _;
  } =
    env.ServerEnv.last_recheck_loop_stats
  in
  ( env,
    {
      did_read_disk_changes = !did_read_disk_changes_ref;
      total_changed_files_count;
      total_rechecked_count;
      last_actual_total_rechecked_count =
        (match env.ServerEnv.last_recheck_loop_stats_for_actual_work with
        | None -> None
        | Some stats ->
          Some stats.ServerEnv.RecheckLoopStats.total_rechecked_count);
      new_client_response =
        TestClientProvider.get_client_response Non_persistent;
    } )

let prepend_root x = root ^ x

let fail x =
  print_endline x;
  Stdlib.Printexc.(get_callstack 100 |> print_raw_backtrace stderr);
  raise Integration_test_failure

(******************************************************************************(
 * Utility functions to help format/throw errors for informative errors
)******************************************************************************)

let relativize (s : string) : string =
  let root_regexp =
    Str.regexp (Str.quote (Relative_path.path_of_prefix Relative_path.Root))
  in
  Str.global_replace root_regexp "/" s

let indent_string_with (indent : string) (error : string) : string =
  indent ^ String.concat ~sep:("\n" ^ indent) Str.(split (regexp "\n") error)

let indent_strings_with (indent : string) (errors : string list) : string =
  String.concat ~sep:"" @@ List.map ~f:(indent_string_with indent) errors

let assertEqual expected got =
  let expected = String.strip expected in
  let got = String.strip got in
  if String.( <> ) expected got then
    fail (Printf.sprintf "Expected:\n%s\n\nBut got:\n%s\n" expected got)

let change_files env disk_changes =
  let (env, loop_output) =
    run_loop_once env { default_loop_input with disk_changes }
  in
  if not loop_output.did_read_disk_changes then
    fail "Expected the server to process disk updates";
  (env, loop_output)

let setup_disk env disk_changes = fst @@ change_files env disk_changes

let error_strings err_list =
  List.map ~f:(fun x -> Errors.to_string (User_error.to_absolute x)) err_list

let assertSingleError expected err_list =
  let error_strings = error_strings err_list in
  match error_strings with
  | [x] -> assertEqual expected x
  | _ ->
    let err_count = List.length err_list in
    let fmt_expected = indent_string_with " < " expected in
    let fmt_actual = indent_strings_with " > " error_strings in
    let msg =
      Printf.sprintf
        "Expected to have exactly one error, namely:

%s

... but got %d errors...

%s
"
        fmt_expected
        err_count
        fmt_actual
    in
    fail msg

let full_check_status env =
  (* the empty string isn't just a test placeholder - when a file is deleted
     or renamed, the content is set to the empty string internally *)
  run_loop_once
    env
    {
      default_loop_input with
      disk_changes = [("__dummy_file_to_trigger_full_check.php", "")];
    }

let start_initial_full_check env =
  (match env.ServerEnv.prechecked_files with
  | ServerEnv.Initial_typechecking _ -> ()
  | _ -> assert false);

  let (env, loop_output) = full_check_status env in
  (match env.ServerEnv.prechecked_files with
  | ServerEnv.Prechecked_files_ready _ -> ()
  | _ -> assert false);

  let { total_rechecked_count; _ } = loop_output in
  (* full_check_status adds a dummy file to trigger a recheck, so we subtract one here
     and return the number of rechecked non-dummy files. *)
  assert (total_rechecked_count >= 1);
  (env, total_rechecked_count - 1)

let errors_to_string buf x =
  List.iter x ~f:(fun error ->
      Printf.bprintf buf "%s\n" (Errors.to_string error))

let assert_errors errors expected =
  let buf = Buffer.create 1024 in
  Errors.get_error_list errors
  |> List.map ~f:User_error.to_absolute
  |> errors_to_string buf;
  assertEqual expected (Buffer.contents buf)

let assert_env_errors env expected = assert_errors env.ServerEnv.errorl expected

let assert_no_errors env = assert_env_errors env ""

let in_daemon f =
  let handle =
    Daemon.fork_FOR_TESTING_ON_UNIX_ONLY
      ~channel_mode:`socket
      Unix.(stdout, stderr)
      (fun () _channels -> f ())
      ()
  in
  match Unix.waitpid [] handle.Daemon.pid with
  | (_, Unix.WEXITED 0) -> ()
  | _ -> assert false

let list_to_string l =
  let buf = Buffer.create 1024 in
  List.iter l ~f:(Printf.bprintf buf "%s ");
  Buffer.contents buf

let assert_ide_completions response expected =
  let results =
    List.map response.AutocompleteTypes.completions ~f:(fun x ->
        x.AutocompleteTypes.res_label)
  in
  let results_as_string = list_to_string results in
  let expected_as_string = list_to_string expected in
  assertEqual expected_as_string results_as_string

let assert_needs_retry loop_output =
  Done_or_retry.(
    match loop_output.new_client_response with
    | Some Retry -> ()
    | Some (Done _) -> fail "Expected needing to retry"
    | None -> fail "Expected response")

let assert_response loop_output =
  Done_or_retry.(
    match loop_output.new_client_response with
    | Some (Done res) -> res
    | Some Retry -> fail "Expecteded not needing to retry"
    | None -> fail "Expected response")

let assert_find_refs loop_output expected =
  let results = assert_response loop_output in
  let results =
    List.map results ~f:(fun (name, pos) -> name ^ ": " ^ Pos.string pos)
  in
  let results_as_string = list_to_string results in
  let expected_as_string = list_to_string expected in
  assertEqual expected_as_string results_as_string

let assert_rename loop_output expected =
  let results = assert_response loop_output in
  (* We don't have any (better than JSON) human-readable format for rename results,
   * and I'm too lazy to write it. Tests will have to compare JSON outputs for now. *)
  let results_as_string = ClientRename.patches_to_json_string results in
  assertEqual expected results_as_string

let assert_needs_recheck env x =
  if
    not
      Relative_path.(
        Set.mem env.ServerEnv.needs_recheck (create_detect_prefix (root ^ x)))
  then
    let () = Printf.eprintf "Expected %s to need recheck\n" x in
    assert false

let assert_needs_no_recheck env x =
  if
    Relative_path.(
      Set.mem env.ServerEnv.needs_recheck (create_detect_prefix (root ^ x)))
  then
    let () = Printf.eprintf "Expected %s not to need recheck\n" x in
    assert false

let doc (suffix : string) (file_contents : string) : ClientIdeMessage.document =
  {
    ClientIdeMessage.file_path =
      Relative_path.from_root ~suffix |> Relative_path.to_absolute |> Path.make;
    file_contents;
  }

let loc (line : int) (column : int) : ClientIdeMessage.location =
  (* 1-based line and column *)
  { ClientIdeMessage.line; column }

module Client = struct
  type env = ClientIdeDaemon.Test.env

  let with_env ~(custom_config : ServerConfig.t option) (f : env -> unit) : unit
      =
    Printexc.record_backtrace true;
    EventLogger.init_fake ();
    Tempfile.with_real_tempdir @@ fun root ->
    Tempfile.with_real_tempdir @@ fun hhi_root ->
    Tempfile.with_real_tempdir @@ fun tmp ->
    let naming_sqlite = Path.concat tmp "naming_table.sql" in
    Relative_path.set_path_prefix Relative_path.Root root;
    Relative_path.set_path_prefix Relative_path.Hhi hhi_root;
    Relative_path.set_path_prefix Relative_path.Tmp tmp;
    let (_save_result : Naming_sqlite.save_result) =
      Naming_sqlite.save_file_infos
        (Path.to_string naming_sqlite)
        Relative_path.Map.empty
        ~base_content_version:""
    in
    let env = ClientIdeDaemon.Test.init ~custom_config ~naming_sqlite in
    f env;
    ()

  let setup_disk (env : env) (files_and_contents : (string * string) list) : env
      =
    let files_and_contents =
      List.map files_and_contents ~f:(fun (suffix, contents) ->
          (Relative_path.from_root ~suffix, contents))
    in
    List.iter files_and_contents ~f:(fun (path, contents) ->
        let file = Relative_path.to_absolute path in
        RealDisk.write_file ~file ~contents;
        TestDisk.set file contents);
    ClientIdeDaemon.Test.index
      env
      (Relative_path.Set.of_list (List.map files_and_contents ~f:fst))

  let edit_file (env : env) (suffix : string) (contents : string) :
      env * ClientIdeMessage.diagnostic list SMap.t =
    let message = ClientIdeMessage.(Did_open_or_change (doc suffix contents)) in
    let (env, ()) = ClientIdeDaemon.Test.handle env message in
    let message = ClientIdeMessage.(Diagnostics (doc suffix contents)) in
    let (env, diagnostics) = ClientIdeDaemon.Test.handle env message in
    let diagnostics = FileMap.singleton ("/" ^ suffix) diagnostics in
    (env, diagnostics)

  let open_file (env : env) (suffix : string) :
      env * ClientIdeMessage.diagnostic list SMap.t =
    let contents =
      TestDisk.get (Relative_path.from_root ~suffix |> Relative_path.to_absolute)
    in
    edit_file env suffix contents

  let close_file (env : env) (suffix : string) :
      env * ClientIdeMessage.diagnostic list SMap.t =
    let message =
      ClientIdeMessage.(
        Did_close
          (Relative_path.from_root ~suffix
          |> Relative_path.to_absolute
          |> Path.make))
    in
    let (env, diagnostics) = ClientIdeDaemon.Test.handle env message in
    let diagnostics = FileMap.singleton ("/" ^ suffix) diagnostics in
    (env, diagnostics)

  let diagnostics_to_string buf x =
    List.iter x ~f:(fun diagnostic ->
        (* For testing we just compare the error itself. Other tests
           exercise full diagnostics information *)
        Printf.bprintf
          buf
          "%s\n"
          (Errors.to_string diagnostic.ClientIdeMessage.diagnostic_error))

  let diagnostics_to_string (x : ClientIdeMessage.diagnostic list SMap.t) =
    let buf = Buffer.create 1024 in
    SMap.iter x ~f:(fun path diagnostics ->
        Printf.bprintf buf "%s:\n" path;
        diagnostics_to_string buf diagnostics);
    Buffer.contents buf

  let assert_no_diagnostics
      (diagnostics : ClientIdeMessage.diagnostic list SMap.t) =
    let is_any =
      FileMap.exists diagnostics ~f:(fun _file d -> not (List.is_empty d))
    in
    if is_any then begin
      let diagnostics_as_string =
        diagnostics_to_string diagnostics |> relativize
      in
      fail
        ("Did not expect to receive IDE diagnostics. Got:\n"
        ^ diagnostics_as_string)
    end

  let assert_diagnostics_string
      (diagnostics : ClientIdeMessage.diagnostic list SMap.t)
      (expected : string) : unit =
    let diagnostics_as_string =
      diagnostics_to_string diagnostics |> relativize
    in
    assertEqual expected diagnostics_as_string
end
