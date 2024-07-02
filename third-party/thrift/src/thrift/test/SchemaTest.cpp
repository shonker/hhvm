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

#include <functional>
#include <string>
#include <vector>
#include <folly/portability/GTest.h>
#include <thrift/annotation/gen-cpp2/thrift_constants.h>
#include <thrift/lib/cpp2/protocol/Serializer.h>
#include <thrift/lib/thrift/gen-cpp2/schema_types.h>
#include <thrift/test/gen-cpp2/schema_constants.h>

using namespace apache::thrift;
using namespace facebook::thrift;
using namespace facebook::thrift::test::schema;

namespace {
template <typename T, typename = void>
struct has_schema {
  constexpr static bool value = false;
};
template <typename T>
struct has_schema<T, std::void_t<decltype(T::_fbthrift_schema)>> {
  constexpr static bool value = true;
};
} // namespace

TEST(SchemaTest, not_linked) {
  EXPECT_FALSE(has_schema<annotation::thrift_constants>::value);
}

TEST(SchemaTest, linked) {
  EXPECT_TRUE(has_schema<schema_constants>::value);
  EXPECT_FALSE(schema_constants::_fbthrift_schema().empty());
  auto schema = apache::thrift::CompactSerializer::deserialize<type::Schema>(
      schema_constants::_fbthrift_schema());

  EXPECT_EQ(schema.programs()[0].path(), "thrift/test/schema.thrift");

  // Includes definitions from root program
  EXPECT_EQ(
      schema.definitionsMap()[schema.programs()[0].definitionKeys()[0]]
          .structDef_ref()
          ->name(),
      "Empty");

  // Only includes definitions from root program
  EXPECT_EQ(
      schema.definitionsMap()->size(),
      schema.programs()[0].definitionKeys()->size());

  // Transitive includes are listed
  EXPECT_GT(schema.programs()->size(), 4);
}
