(*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)
open Hh_prelude
module CodeAction = Lsp.CodeAction

let apply_patches_to_string old_content (patches : ServerRenameTypes.patch list)
    : string =
  let buf = Buffer.create (String.length old_content) in
  let patch_list =
    List.sort ~compare:ServerRenameTypes.compare_result patches
  in
  ServerRenameTypes.write_patches_to_buffer buf old_content patch_list;
  Buffer.contents buf

let lsp_range_to_pos ~source_text path range =
  let line_to_offset line =
    Full_fidelity_source_text.position_to_offset source_text (line, 0)
  in
  (* Correct for inconsistencies in 0-vs-1-indexing elsewhere.
     TODO(T154897774): fix off-by-ones in conversion functions and remove this adaptor
  *)
  let range =
    Lsp.
      {
        start =
          { line = range.start.line + 1; character = range.start.character + 1 };
        end_ =
          { line = range.end_.line + 1; character = range.end_.character + 1 };
      }
  in
  Lsp_helpers.lsp_range_to_pos ~line_to_offset path range |> Pos.to_absolute

let extract_cursor_position_for_code_action
    ~source_text path (code_action : _ CodeAction.command_or_action_) :
    (Pos.absolute option, string) Result.t =
  match code_action with
  | CodeAction.Action CodeAction.{ action = BothEditThenCommand (_, c); _ } ->
    Result.map
      (Lsp_extra_commands.parse_set_selection c)
      ~f:(Option.map ~f:(lsp_range_to_pos ~source_text path))
  | _ -> Ok None

let patched_text_of_command_or_action ~source_text path code_action :
    (string option, string) Result.t =
  match code_action with
  | CodeAction.Action
      CodeAction.{ action = EditOnly Lsp.WorkspaceEdit.{ changes }; _ }
  | CodeAction.Action
      CodeAction.
        { action = BothEditThenCommand (Lsp.WorkspaceEdit.{ changes }, _); _ }
    ->
    let open Result.Let_syntax in
    let to_patch Lsp.TextEdit.{ range; newText = text } =
      let pos = lsp_range_to_pos ~source_text path range in
      ServerRenameTypes.Replace ServerRenameTypes.{ pos; text }
    in
    let patches =
      Lsp.DocumentUri.Map.values changes |> List.concat |> List.map ~f:to_patch
    in
    (* The new cursor position is relative to the content *after* patching.
       So first patch... *)
    let source_text = Sys_utils.cat @@ Relative_path.to_absolute path in
    let rewritten_contents = apply_patches_to_string source_text patches in
    (* ... and only then indicate the cursor position *)
    let* cursor_position =
      extract_cursor_position_for_code_action
        ~source_text:(Full_fidelity_source_text.make path rewritten_contents)
        path
        code_action
    in
    let rewritten_contents =
      match cursor_position with
      | None -> rewritten_contents
      | Some selection ->
        let patches =
          ServerRenameTypes.
            [
              Replace { pos = Pos.shrink_to_start selection; text = ">|" };
              Replace { pos = Pos.shrink_to_end selection; text = "|<" };
            ]
        in
        apply_patches_to_string rewritten_contents patches
    in
    Ok (Some rewritten_contents)
  | CodeAction.Action _
  | CodeAction.Command _ ->
    Ok None

let run_exn ctx entry range ~title_prefix ~use_snippet_edits =
  let commands_or_actions = Code_actions_services.go ~ctx ~entry ~range in
  if List.is_empty commands_or_actions then
    Printf.printf "No commands or actions found\n"
  else begin
    let description_triples =
      commands_or_actions
      |> List.map ~f:(function
             | CodeAction.Action CodeAction.{ title; kind; _ } ->
               let kind_str =
                 Printf.sprintf "CodeActionKind: %s"
                 @@ Lsp.CodeActionKind.string_of_kind kind
               in
               let is_selected = String.is_prefix ~prefix:title_prefix title in
               (title, kind_str, is_selected)
             | CodeAction.Command Lsp.Command.{ title; _ } ->
               let is_selected = String.is_prefix ~prefix:title_prefix title in
               (title, "Command", is_selected))
    in
    let separator = "\n------------------------------------------\n" in
    Printf.printf "Code actions available:%s" separator;
    description_triples
    |> List.iter ~f:(fun (title, kind_str, is_selected) ->
           let selected_str =
             if is_selected then
               " SELECTED"
             else
               ""
           in
           Printf.printf "%s (%s)%s\n" title kind_str selected_str);
    let selected_titles =
      description_triples
      |> List.filter_map ~f:(fun (title, _, is_selected) ->
             Option.some_if is_selected title)
    in
    let output_code_action selected_title =
      let resolved =
        Code_actions_services.resolve
          ~ctx
          ~entry
          ~range
          ~resolve_title:selected_title
          ~use_snippet_edits
        |> Result.map_error ~f:(fun e ->
               Hh_json.json_to_string ~sort_keys:true ~pretty:true
               @@ Lsp_fmt.print_error e)
        |> Result.ok_or_failwith
      in
      let hermeticize_paths =
        Str.global_replace (Str.regexp "\".+?.php\"") "\"FILE.php\""
      in
      let source_text =
        entry.Provider_context.source_text |> Option.value_exn
      in
      let text_of_selected =
        let patched_opt =
          patched_text_of_command_or_action
            ~source_text
            entry.Provider_context.path
            resolved
        in
        match patched_opt with
        | Ok (Some patched) ->
          Printf.sprintf "\nApplied edit for code action:%s%s" separator patched
        | Ok None ->
          "\nThe command_or_action cannot be converted into patches.\n"
        | Error error ->
          Printf.sprintf
            "\nError while converting the command_or_action into patches: %s"
            error
      in
      Printf.printf "\nJSON for selected code action:%s" separator;
      resolved
      |> Result.return
      |> Lsp_fmt.print_codeActionResolveResult
      |> Hh_json.json_to_string ~sort_keys:true ~pretty:true
      |> hermeticize_paths
      |> Printf.printf "%s\n";
      Printf.printf "%s\n" text_of_selected
    in
    match selected_titles with
    | [selected_title] -> output_code_action selected_title
    | _ :: _ ->
      begin
        Printf.printf
          "\nMultiple code action titles match prefix: '%s'\n"
          title_prefix
      end;
      List.iter selected_titles ~f:(fun selected_title ->
          Printf.printf "\nCode action title: %s\n" selected_title;
          output_code_action selected_title)
    | [] ->
      Printf.printf "\nNo code action titles match prefix: %s\n" title_prefix
  end

let run ctx entry range ~title_prefix ~use_snippet_edits =
  match run_exn ctx entry range ~title_prefix ~use_snippet_edits with
  | exception exn -> print_endline @@ Exn.to_string exn
  | () -> ()
