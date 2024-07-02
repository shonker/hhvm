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

#include <thrift/lib/thrift/detail/protocol.h>

#include <folly/Hash.h>
#include <folly/Indestructible.h>
#include <folly/io/IOBuf.h>
#include <thrift/conformance/cpp2/AnyRegistry.h>
#include <thrift/conformance/cpp2/ThriftTypeInfo.h>
#include <thrift/lib/thrift/gen-cpp2/protocol_detail_types.h>

#include <xxhash.h>

namespace apache::thrift::protocol::detail {

template <class Base>
const char* ObjectWrapper<Base>::__fbthrift_thrift_uri() {
  static const folly::Indestructible<std::string> ret = uri<Base>();
  return ret->c_str();
}

template <class Base>
const char* ValueWrapper<Base>::__fbthrift_thrift_uri() {
  static const folly::Indestructible<std::string> ret = uri<Base>();
  return ret->c_str();
}

static std::size_t hashVector(const std::vector<std::size_t>& a) {
  return XXH3_64bits(a.data(), a.size() * sizeof(a[0]));
}

std::size_t hash_value(const Value& s) {
  auto value_type = s.getType();
  switch (value_type) {
    case Value::Type::boolValue:
      return folly::hash::hash_combine(value_type, s.as_bool());
    case Value::Type::byteValue:
      return folly::hash::hash_combine(value_type, s.as_byte());
    case Value::Type::i16Value:
      return folly::hash::hash_combine(value_type, s.as_i16());
    case Value::Type::i32Value:
      return folly::hash::hash_combine(value_type, s.as_i32());
    case Value::Type::i64Value:
      return folly::hash::hash_combine(value_type, s.as_i64());
    case Value::Type::floatValue:
      return folly::hash::hash_combine(value_type, s.as_float());
    case Value::Type::doubleValue:
      return folly::hash::hash_combine(value_type, s.as_double());
    case Value::Type::stringValue:
      return folly::hash::hash_combine(value_type, s.as_string());
    case Value::Type::binaryValue:
      return folly::hash::hash_combine(
          value_type, folly::IOBufHash()(s.as_binary()));
    case Value::Type::listValue: {
      const auto& list = *s.listValue_ref();
      std::vector<std::size_t> hashes;
      hashes.reserve(list.size() + 1);
      hashes.push_back(static_cast<std::size_t>(value_type));
      for (auto&& v : list) {
        hashes.push_back(hash_value(v));
      }
      return hashVector(hashes);
    }
    case Value::Type::setValue: {
      const auto& set = *s.setValue_ref();
      std::vector<std::size_t> hashes(set.size() + 1);
      hashes[0] = static_cast<std::size_t>(value_type);
      std::size_t* valueHashes = hashes.data() + 1;
      std::size_t i = 0;
      for (auto&& v : set) {
        valueHashes[i++] = hash_value(v);
      }
      std::sort(valueHashes, hashes.data() + hashes.size());
      return hashVector(hashes);
    }
    case Value::Type::mapValue: {
      // Keys and values are sorted independently, so this produces collisions
      // between maps where the same values are mapped to the same keys
      // differently.
      const auto& map = *s.mapValue_ref();
      std::size_t size = map.size();
      std::vector<std::size_t> hashes(size * 2 + 1);
      hashes[0] = static_cast<std::size_t>(value_type);
      std::size_t* keyHashes = hashes.data() + 1;
      std::size_t* valueHashes = keyHashes + size;
      std::size_t i = 0;
      for (auto&& [k, v] : map) {
        hashes[i] = hash_value(k);
        hashes[size + i] = hash_value(v);
        ++i;
      }
      std::sort(keyHashes, valueHashes);
      std::sort(valueHashes, hashes.data() + hashes.size());
      return hashVector(hashes);
    }
    case Value::Type::objectValue: {
      const Object& object = *s.objectValue_ref();
      size_t size = object.size();
      std::vector<std::size_t> hashes(size * 2 + 2);
      hashes[0] = static_cast<std::size_t>(value_type);
      hashes[1] = std::hash<std::string>{}(*object.type());
      std::size_t* keyHashes = hashes.data() + 2;
      std::size_t* valueHashes = keyHashes + size;
      std::size_t i = 0;
      for (auto&& [k, v] : object) {
        keyHashes[i] = k; // hashing integer key is the identity
        valueHashes[i] = hash_value(v);
        ++i;
      }
      std::sort(keyHashes, valueHashes);
      std::sort(valueHashes, hashes.data() + hashes.size());
      return hashVector(hashes);
    }
    case Value::Type::__EMPTY__: {
      return 0;
    }
  }
  return 0;
}

template const char* ObjectWrapper<detail::Object>::__fbthrift_thrift_uri();
template const char* ValueWrapper<detail::Value>::__fbthrift_thrift_uri();

} // namespace apache::thrift::protocol::detail
