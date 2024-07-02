/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include <thrift/compiler/ast/t_program.h>
#include <thrift/compiler/ast/t_struct.h>
#include <thrift/compiler/ast/t_typedef.h>

namespace apache {
namespace thrift {
namespace compiler {
namespace rust {

struct rust_crate {
  std::vector<std::string> dependency_path;
  bool multifile = false;
  std::string label;

  std::string import_name(const t_program* program) const;
};

struct rust_crate_map {
  bool multifile_mode = false;
  std::string label;
  std::map<std::string, rust_crate> cratemap;
};

class rust_crate_index {
 public:
  rust_crate_index() = default;
  rust_crate_index(
      const t_program* current_program,
      std::map<std::string, rust_crate> cratemap);
  const rust_crate* find(const t_program* program) const;
  std::vector<const rust_crate*> direct_dependencies() const;

 private:
  void compute_absolute_paths_of_includes(
      const t_program* program, const std::string& absolute_path);

  std::map<std::string, rust_crate> cratemap;
  std::map<const t_program*, std::string> thrift_file_absolute_paths;
};

rust_crate_map load_crate_map(const std::string& path);

std::string mangle(const std::string& name);
std::string mangle_crate_name(const std::string& name);
std::string mangle_type(const std::string& name);
std::string snakecase(const std::string& name);
std::string camelcase(const std::string& name);
std::string quote(const std::string& data, bool do_backslash);

bool get_annotation_property_bool(
    const t_const* annotation, const std::string& key);
std::string get_annotation_property_string(
    const t_const* annotation, const std::string& key);

std::string named_rust_name(const t_named* name); // mangle()
std::string type_rust_name(const t_type* type_); // mangle_type()
inline std::string typedef_rust_name(const t_typedef* typedef_) {
  return type_rust_name(typedef_);
}
inline std::string struct_rust_name(const t_structured* struct_) {
  return type_rust_name(struct_);
}

std::string multifile_module_name(const t_program* program);

} // namespace rust
} // namespace compiler
} // namespace thrift
} // namespace apache
