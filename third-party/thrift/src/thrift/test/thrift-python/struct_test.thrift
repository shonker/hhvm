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

include "thrift/annotation/python.thrift"

namespace py3 thrift.test.thrift_python

const bool bool_constant = true;
const byte byte_constant = -10; // byte is an 8-bit signed integer
const i16 i16_constant = 200;
const i32 i32_constant = 0xFA12EE;
const i64 i64_constant = 0xFFFFFFFFFF;
const float float_constant = 2.718281828459;
const double double_constant = 2.718281828459;
const string string_constant = "June 28, 2017";

const list<i32> list_constant = [2, 3, 5, 7];

const set<string> set_constant = ["foo", "bar", "baz"];

const map<string, i32> map_constant = {"foo": 1, "bar": 2};

struct TestStruct {
  1: string unqualified_string;
  2: optional string optional_string;
}

struct TestStructWithDefaultValues {
  1: i32 unqualified_integer = 42;

  2: optional i32 optional_integer = 43;

  3: TestStruct unqualified_struct = TestStruct{unqualified_string = "hello"};

  4: optional TestStruct optional_struct = TestStruct{
    unqualified_string = "world",
  };

  5: TestStruct unqualified_struct_intrinsic_default;

  6: optional TestStruct optional_struct_intrinsic_default;
}

struct TestStructAllThriftPrimitiveTypes {
  1: bool unqualified_bool;
  2: optional bool optional_bool;

  3: byte unqualified_byte;
  4: optional byte optional_byte;

  5: i16 unqualified_i16;
  6: optional i16 optional_i16;

  7: i32 unqualified_i32;
  8: optional i32 optional_i32;

  9: i64 unqualified_i64;
  10: optional i64 optional_i64;

  11: float unqualified_float;
  12: optional float optional_float;

  13: double unqualified_double;
  14: optional double optional_double;

  15: string unqualified_string;
  16: optional string optional_string;
}

struct TestStructAllThriftPrimitiveTypesWithDefaultValues {
  1: bool unqualified_bool = true;

  2: byte unqualified_byte = 32;

  3: i16 unqualified_i16 = 512;

  4: i32 unqualified_i32 = 2048;

  5: i64 unqualified_i64 = 999;

  6: float unqualified_float = 1.0;

  7: double unqualified_double = 1.231;

  8: string unqualified_string = "thrift-python";
}

struct TestStructAllThriftContainerTypes {
  1: list<i32> unqualified_list_i32;
  2: optional list<i32> optional_list_i32;

  3: set<string> unqualified_set_string;
  4: optional set<string> optional_set_string;

  5: map<string, i32> unqualified_map_string_i32;
  6: optional map<string, i32> optional_map_string_i32;
}

struct TestStructAdaptedTypes {
  @python.Adapter{
    name = "thrift.python.test.adapters.datetime.DatetimeAdapter",
    typeHint = "datetime.datetime",
  }
  1: i32 unqualified_adapted_i32_to_datetime;

  @python.Adapter{
    name = "thrift.python.test.adapters.datetime.DatetimeAdapter",
    typeHint = "datetime.datetime",
  }
  2: optional i32 optional_adapted_i32_to_datetime;

  @python.Adapter{
    name = "thrift.python.test.adapters.atoi.AtoiAdapter",
    typeHint = "int",
  }
  3: string unqualified_adapted_string_to_i32 = "123";
}

struct TestStructEmpty {}
typedef TestStructEmpty TestStructEmptyAlias

struct TestStructWithTypedefField {
  1: i32 n;
  2: TestStructEmpty empty_struct;
  3: TestStructEmptyAlias empty_struct_alias;
}

struct TestStructNested_2 {
  1: i32 i32_field;
}

struct TestStructNested_1 {
  1: i32 i32_field;
  2: TestStructNested_2 nested_2;
}

struct TestStructNested_0 {
  1: i32 i32_field;
  2: TestStructNested_1 nested_1;
}

union TestUnion {
  1: i32 i32_field;
  2: string string_field;
}

struct TestStructWithUnionField {
  1: i32 i32_field;
  2: TestUnion union_field;
}
