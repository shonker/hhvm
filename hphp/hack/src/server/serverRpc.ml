(*
 * Copyright (c) 2015, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

open Hh_prelude
open ServerEnv
open ServerCommandTypes

let remove_dead_warning name =
  "hh_server was started without '--no-load', which is required when removing dead "
  ^ name
  ^ "s.\n"
  ^ "Please run 'hh_client restart --no-load' to restart it."

let take_max_errors max_errors error_list =
  match max_errors with
  | Some max_errors ->
    let (error_list, dropped_errors) = List.split_n error_list max_errors in
    (error_list, List.length dropped_errors)
  | None -> (error_list, 0)

let single_ctx env path file_input =
  let contents =
    match file_input with
    | ServerCommandTypes.FileName path -> Sys_utils.cat path
    | ServerCommandTypes.FileContent contents -> contents
  in
  let ctx = Provider_utils.ctx_from_server_env env in
  Provider_context.add_or_overwrite_entry_contents ~ctx ~path ~contents

let single_ctx_path env path =
  single_ctx
    env
    (Relative_path.create_detect_prefix path)
    (ServerCommandTypes.FileName path)

let log_check_response env =
  HackEventLogger.check_response
    (Errors.get_error_list env.errorl
    |> List.map ~f:(fun { User_error.code; _ } -> code))

(* Might raise {!Naming_table.File_info_not_found} *)
let handle : type a. genv -> env -> is_stale:bool -> a t -> env * a =
 fun genv env ~is_stale -> function
  | STATUS { max_errors; _ } ->
    log_check_response env;
    let error_list = Errors.sort_and_finalize env.errorl in
    let (error_list, dropped_count) = take_max_errors max_errors error_list in
    let liveness =
      if is_stale then
        Stale_status
      else
        Live_status
    in
    let last_recheck_stats =
      match env.ServerEnv.last_recheck_loop_stats_for_actual_work with
      | None -> None
      | Some recheck_stats ->
        Some
          (ServerEnv.RecheckLoopStats.to_user_telemetry recheck_stats
          |> Telemetry.string_ ~key:"init_id" ~value:env.init_env.init_id)
    in
    ( env,
      { Server_status.liveness; error_list; dropped_count; last_recheck_stats }
    )
  | STATUS_SINGLE
      { file_names; max_errors; preexisting_warnings; return_expanded_tast } ->
    let ctx = Provider_utils.ctx_from_server_env env in
    let (errors, tasts) = ServerStatusSingle.go file_names ctx in
    let errors =
      errors
      |> ServerTypeCheck.filter_out_mergebase_warnings env ~preexisting_warnings
      |> Errors.get_sorted_error_list
      |> List.map ~f:User_error.to_absolute
      |> take_max_errors max_errors
    in
    (* Unforced lazy values are closures which make serialization over RPC fail. *)
    let tasts =
      if return_expanded_tast then
        Some
          (Relative_path.Map.map
             tasts
             ~f:
               (Tast_with_dynamic.map ~f:(fun tast ->
                    tast
                    |> Tast_expand.expand_program ctx
                    |> Tast.force_lazy_values)))
      else
        None
    in
    (env, (errors, tasts))
  | INFER_TYPE (file_input, line, column) ->
    let path =
      match file_input with
      | FileName fn -> Relative_path.create_detect_prefix fn
      | FileContent _ -> Relative_path.create_detect_prefix ""
    in
    let (ctx, entry) = single_ctx env path file_input in
    let result =
      Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
          ServerInferType.go_ctx ~ctx ~entry ~line ~column)
    in
    (env, result)
  | INFER_TYPE_BATCH positions ->
    (env, ServerInferTypeBatch.go genv.workers positions env)
  | IS_SUBTYPE stdin -> (env, ServerIsSubtype.check genv.workers stdin env)
  | TAST_HOLES (file_input, hole_filter) ->
    let path =
      match file_input with
      | FileName fn -> Relative_path.create_detect_prefix fn
      | FileContent _ -> Relative_path.create_detect_prefix ""
    in
    let (ctx, entry) = single_ctx env path file_input in
    let result =
      Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
          ServerCollectTastHoles.go_ctx ~ctx ~entry ~hole_filter)
    in
    (env, result)
  | TAST_HOLES_BATCH files ->
    (env, ServerTastHolesBatch.go genv.workers files env)
  | INFER_TYPE_ERROR (file_input, line, column) ->
    let path =
      match file_input with
      | FileName fn -> Relative_path.create_detect_prefix fn
      | FileContent _ -> Relative_path.create_detect_prefix ""
    in
    let (ctx, entry) = single_ctx env path file_input in
    let result =
      Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
          ServerInferTypeError.go_ctx ~ctx ~entry ~line ~column)
    in
    (env, result)
  (* TODO: edit this to look for classname *)
  | XHP_AUTOCOMPLETE_SNIPPET cls ->
    let ctx = Provider_utils.ctx_from_server_env env in
    let tast_env = Tast_env.empty ctx in
    let cls = Utils.add_ns cls in
    (env, AutocompleteService.get_snippet_for_xhp_classname cls ctx tast_env)
  | IDENTIFY_SYMBOL arg ->
    let module SO = SymbolOccurrence in
    let ctx = Provider_utils.ctx_from_server_env env in
    let get_def_opt type_ name =
      ServerSymbolDefinition.go
        ctx
        None
        SO.{ type_; name; is_declaration = false; pos = Pos.none }
      |> Option.to_list
      |> List.map ~f:SymbolDefinition.to_absolute
    in
    let arg = Str.split (Str.regexp "::") arg in
    (* The following are all the different named entities I could think of in Hack. *)
    let results =
      match arg with
      | [c_name; member] ->
        let c_name = Utils.add_ns c_name in
        List.concat
          [
            get_def_opt (SO.Method (SO.ClassName c_name, member)) "";
            get_def_opt (SO.Property (SO.ClassName c_name, member)) "";
            get_def_opt (SO.XhpLiteralAttr (c_name, member)) "";
            get_def_opt (SO.ClassConst (SO.ClassName c_name, member)) "";
            get_def_opt (SO.Typeconst (c_name, member)) "";
          ]
      | [name] ->
        let name = Utils.add_ns name in
        List.concat
          [
            get_def_opt (SO.Class SO.ClassId) name;
            (* SO.Record and Class find the same things *)
            get_def_opt SO.Function name;
            get_def_opt SO.GConst name;
          ]
      | _ -> []
    in
    (env, results)
  | IDENTIFY_FUNCTION (filename, file_input, line, column) ->
    let (ctx, entry) =
      single_ctx env (Relative_path.create_detect_prefix filename) file_input
    in
    let result =
      Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
          ServerIdentifyFunction.go_quarantined_absolute
            ~ctx
            ~entry
            ~line
            ~column)
    in
    (env, result)
  | METHOD_JUMP (class_, filter, find_children) ->
    Printf.printf "%s" class_;
    let ctx = Provider_utils.ctx_from_server_env env in
    ( env,
      MethodJumps.get_inheritance
        ctx
        class_
        ~filter
        ~find_children
        env.naming_table
        genv.workers )
  | METHOD_JUMP_BATCH (classes, filter) ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerMethodJumpsBatch.go ctx genv.workers classes filter)
  | FIND_REFS find_refs_action ->
    let ctx = Provider_utils.ctx_from_server_env env in
    Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
        let open Done_or_retry in
        let include_defs = false in
        ServerFindRefs.(
          go
            ctx
            find_refs_action
            include_defs
            ~stream_file:None
            ~hints:[]
            genv
            env
          |> map_env ~f:to_absolute))
  | GO_TO_IMPL go_to_impl_action ->
    Done_or_retry.(
      ServerGoToImpl.go ~action:go_to_impl_action ~genv ~env
      |> map_env ~f:ServerFindRefs.to_absolute)
  | IDE_FIND_REFS_BY_SYMBOL
      {
        FindRefsWireFormat.CliArgs.symbol_name = _;
        action;
        stream_file;
        hint_suffixes;
      } ->
    let hints =
      List.map hint_suffixes ~f:(fun suffix -> Relative_path.from_root ~suffix)
    in
    let ctx = Provider_utils.ctx_from_server_env env in
    Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
        let open Done_or_retry in
        let include_defs = false in
        map_env
          ~f:ServerFindRefs.to_absolute
          (ServerFindRefs.go
             ctx
             action
             include_defs
             ~stream_file
             ~hints
             genv
             env))
  | IDE_GO_TO_IMPL_BY_SYMBOL
      { FindRefsWireFormat.CliArgs.symbol_name = _; action; _ } ->
    let ctx = Provider_utils.ctx_from_server_env env in
    Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
        let open Done_or_retry in
        map_env
          ~f:ServerFindRefs.to_absolute
          (ServerGoToImpl.go ~action ~genv ~env))
  | RENAME rename_action ->
    let ctx = Provider_utils.ctx_from_server_env env in
    Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
        let definition_for_wrapper =
          match rename_action with
          | ServerRenameTypes.ClassRename _
          | ServerRenameTypes.ClassConstRename _
          | ServerRenameTypes.LocalVarRename _ ->
            None
          | ServerRenameTypes.MethodRename { class_name; old_name; _ } ->
            ServerSymbolDefinition.go
              ctx
              None
              {
                SymbolOccurrence.name = "unused for lookup";
                type_ =
                  SymbolOccurrence.Method
                    ( SymbolOccurrence.ClassName (Utils.add_ns class_name),
                      old_name );
                is_declaration = false;
                pos = Pos.none;
              }
          | ServerRenameTypes.FunctionRename { old_name; _ } ->
            ServerSymbolDefinition.go
              ctx
              None
              {
                SymbolOccurrence.name = Utils.add_ns old_name;
                type_ = SymbolOccurrence.Function;
                is_declaration = false;
                pos = Pos.none;
              }
        in
        ServerRename.go ctx rename_action genv env ~definition_for_wrapper)
  | IDE_RENAME_BY_SYMBOL (action, new_name, symbol_definition) ->
    let ctx = Provider_utils.ctx_from_server_env env in
    Provider_utils.respect_but_quarantine_unsaved_changes ~ctx ~f:(fun () ->
        let open Done_or_retry in
        match
          ServerRename.go_ide_with_find_refs_action
            ctx
            ~find_refs_action:action
            ~new_name
            ~symbol_definition
            genv
            env
        with
        | Error e -> (env, Done (Error e))
        | Ok r -> map_env r ~f:(fun x -> Ok x))
  | REMOVE_DEAD_FIXMES codes ->
    if genv.ServerEnv.options |> ServerArgs.no_load then (
      log_check_response env;
      (env, `Ok (ServerRename.get_fixme_patches codes env))
    ) else
      (env, `Error (remove_dead_warning "fixme"))
  | REMOVE_DEAD_UNSAFE_CASTS ->
    if genv.ServerEnv.options |> ServerArgs.no_load then (
      log_check_response env;
      (env, `Ok (ServerRename.get_dead_unsafe_cast_patches env))
    ) else
      (env, `Error (remove_dead_warning "unsafe cast"))
  | REWRITE_LAMBDA_PARAMETERS files ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerRename.get_lambda_parameter_rewrite_patches ctx files)
  | DUMP_SYMBOL_INFO file_list ->
    (env, SymbolInfoService.go genv.workers file_list env)
  | IN_MEMORY_DEP_TABLE_SIZE ->
    (* TODO(hverr): Clean up 32-bit/migrate *)
    (env, Ok 0)
  | SAVE_NAMING filename ->
    (env, SaveStateService.go_naming env.naming_table filename)
  | SAVE_STATE (filename, gen_saved_ignore_type_errors) ->
    if Errors.is_empty env.errorl || gen_saved_ignore_type_errors then
      (env, SaveStateService.go env filename)
    else
      (env, Error "There are typecheck errors; cannot generate saved state.")
  | CHECK_LIVENESS ->
    (* This is for the client to know "is the server available to process requests?" *)
    (env, ())
  | LINT fnl ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerLint.go genv ctx fnl)
  | LINT_STDIN { filename; contents } ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerLint.go_stdin ctx ~filename ~contents)
  | LINT_ALL code ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerLint.lint_all genv ctx code)
  | STATS -> (env, Stats.get_stats ())
  | FORMAT (content, from, to_) ->
    let legacy_format_options =
      { Lsp.DocumentFormatting.tabSize = 2; insertSpaces = true }
    in
    (env, ServerFormat.go ~content from to_ legacy_format_options)
  | DUMP_FULL_FIDELITY_PARSE file -> (env, FullFidelityParseService.go file)
  | RAGE -> (env, ServerRage.go genv env)
  | CST_SEARCH { sort_results; input; files_to_search } -> begin
    try
      (env, CstSearchService.go genv env ~sort_results ~files_to_search input)
    with
    | MultiThreadedCall.Coalesced_failures failures ->
      let failures =
        failures
        |> List.map ~f:WorkerController.failure_to_string
        |> String.concat ~sep:"\n"
      in
      ( env,
        Error
          (Printf.sprintf
             "Worker failures - check the logs for more details:\n%s\n"
             failures) )
    | exn ->
      let e = Exception.wrap exn in
      (env, Error (Exception.to_string e))
  end
  | NO_PRECHECKED_FILES -> (ServerPrecheckedFiles.expand_all env, ())
  | FUN_DEPS_BATCH positions ->
    (env, ServerFunDepsBatch.go genv.workers positions env)
  | LIST_FILES_WITH_ERRORS -> (env, ServerEnv.list_files env)
  | FILE_DEPENDENTS filenames ->
    let files = ServerFileDependents.go genv env filenames in
    (env, files)
  | IDENTIFY_TYPES (labelled_file, line, column) ->
    let (path, file_input) =
      ServerCommandTypesUtils.extract_labelled_file labelled_file
    in
    let (ctx, entry) = single_ctx env path file_input in
    let result =
      ServerTypeDefinition.go_quarantined ~ctx ~entry ~line ~column
    in
    (env, result)
  | VERBOSE verbose ->
    if verbose then
      Hh_logger.Level.set_min_level Hh_logger.Level.Debug
    else
      Hh_logger.Level.set_min_level
        genv.local_config.ServerLocalConfig.min_log_level;
    (env, ())
  | DEPS_OUT_BATCH positions ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerDepsOutBatch.go ctx positions)
  | DEPS_IN_BATCH positions ->
    let ctx = Provider_utils.ctx_from_server_env env in
    (env, ServerDepsInBatch.go ~ctx ~genv ~env positions)
