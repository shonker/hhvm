# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# pyre-strict

from thrift.test.python_capi.thrift_dep.thrift_types import DepEnum, DepStruct, DepUnion

def enum_from_cpp() -> DepEnum: ...
def struct_from_cpp() -> DepStruct: ...
def union_from_cpp() -> DepUnion: ...
def unpack_python_enum(x: object) -> int: ...
def unpack_python_struct(x: object) -> str: ...
def unpack_python_union(x: object) -> str: ...
def unpack_python_union_throws(x: object) -> str: ...
