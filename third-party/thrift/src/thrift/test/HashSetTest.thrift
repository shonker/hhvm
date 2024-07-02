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

namespace cpp2 apache.thrift.test

struct foo {
  1: set_i32_4564 bar;
  2: set_string_8797 baz;
}

// The following were automatically generated and may benefit from renaming.
typedef set<i32> (cpp.template = "std::unordered_set") set_i32_4564
typedef set<string> (cpp.template = "std::unordered_set") set_string_8797
