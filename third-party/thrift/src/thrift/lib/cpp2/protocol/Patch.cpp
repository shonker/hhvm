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

#include <thrift/lib/cpp2/protocol/Patch.h>

#include <cmath>
#include <limits>
#include <stdexcept>

#include <fmt/core.h>
#include <folly/MapUtil.h>
#include <folly/io/IOBufQueue.h>
#include <folly/lang/Exception.h>
#include <thrift/lib/cpp/util/EnumUtils.h>
#include <thrift/lib/cpp/util/SaturatingMath.h>
#include <thrift/lib/cpp/util/VarintUtils.h>
#include <thrift/lib/cpp2/op/Get.h>
#include <thrift/lib/cpp2/protocol/BinaryProtocol.h>
#include <thrift/lib/cpp2/protocol/CompactProtocol.h>
#include <thrift/lib/cpp2/protocol/FieldMask.h>
#include <thrift/lib/cpp2/protocol/Object.h>
#include <thrift/lib/cpp2/type/Id.h>
#include <thrift/lib/cpp2/type/NativeType.h>
#include <thrift/lib/cpp2/type/Tag.h>
#include <thrift/lib/thrift/gen-cpp2/patch_op_types.h>
#include <thrift/lib/thrift/gen-cpp2/protocol_types.h>

namespace apache {
namespace thrift {
namespace protocol {
namespace detail {
namespace {

using op::PatchOp;

template <typename Tag>
using value_field_id =
    type::field_id_tag<static_cast<FieldId>(type::base_type_v<Tag>)>;

template <typename Tag>
using value_native_type = op::get_native_type<Value, value_field_id<Tag>>;

constexpr FieldId kSafePatchVersionId = FieldId{1};
constexpr FieldId kSafePatchDataId = FieldId{2};

void checkNotSafePatch(const Object& patch) {
  const Value* version = patch.if_contains(kSafePatchVersionId);
  const Value* data = patch.if_contains(kSafePatchDataId);
  if (version && version->is_i32() && data && data->is_binary()) {
    folly::throw_exception<std::runtime_error>(
        "Safe Patch provided. Use `fromSafePatch` to convert to Dynamic Patch first.");
  }
}

PatchOp toOp(FieldId id) {
  auto op = static_cast<PatchOp>(id);
  if (util::enumName<PatchOp>(op) == nullptr) {
    folly::throw_exception<std::runtime_error>(
        fmt::format("Unknown operation id found in patch object: {}", id));
  }
  return op;
}

void checkOps(
    const Object& patch,
    Value::Type valueType,
    folly::F14FastSet<PatchOp> supportedOps) {
  checkNotSafePatch(patch);
  for (const auto& field : patch) {
    auto op = toOp(FieldId{field.first});
    if (supportedOps.find(op) == supportedOps.end()) {
      folly::throw_exception<std::runtime_error>(fmt::format(
          "Unsupported op: {}({})",
          util::enumNameSafe<PatchOp>(op),
          util::enumNameSafe<Value::Type>(valueType)));
    }
  }
}

const Value* findOp(const Object& patch, PatchOp op) {
  return patch.if_contains(static_cast<FieldId>(op));
}

Value* findOp(Object&& patch, PatchOp op) {
  return patch.if_contains(static_cast<FieldId>(op));
}

template <typename Tag, typename V>
decltype(auto) argAs(V&& value) {
  static_assert(std::is_same_v<folly::remove_cvref_t<V>, Value>);
  using Id = type::field_id_tag<static_cast<FieldId>(type::base_type_v<Tag>)>;
  constexpr auto expected = static_cast<Value::Type>(Id::value);
  if (value.getType() != Value::Type(Id::value)) {
    folly::throw_exception<std::runtime_error>(fmt::format(
        "Unexpected type in the patch. Expected {} got {}",
        util::enumNameSafe<Value::Type>(expected),
        util::enumNameSafe<Value::Type>(value.getType())));
  }
  return *op::get<Id, Value>(std::forward<V>(value));
}

template <typename Tag>
bool applyAssign(const Object& patch, value_native_type<Tag>& value) {
  if (const Value* arg = findOp(patch, PatchOp::Assign)) {
    value = argAs<Tag>(*arg);
    return true;
  }
  return false;
}

template <typename Tag>
bool applyAssign(Object&& patch, value_native_type<Tag>& value) {
  if (Value* arg = findOp(std::move(patch), PatchOp::Assign)) {
    value = argAs<Tag>(std::move(*arg));
    return true;
  }
  return false;
}

template <typename Tag, typename T>
void applyNumericPatch(const Object& patch, T& value) {
  constexpr auto valueType = static_cast<Value::Type>(type::base_type_v<Tag>);
  checkOps(patch, valueType, {PatchOp::Assign, PatchOp::Clear, PatchOp::Add});
  if (applyAssign<Tag>(patch, value)) {
    return; // Ignore all other ops.
  }
  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (argAs<type::bool_t>(*clear)) {
      value = {};
    }
  }

  if (auto* arg = findOp(patch, PatchOp::Add)) {
    value = util::add_saturating<T>(value, argAs<Tag>(*arg));
  }
}

} // namespace

void ApplyPatch::operator()(const Object& patch, protocol::Value& value) const {
  // TODO(afuller): Consider using visitation instead.
  switch (value.getType()) {
    case Value::Type::boolValue:
      return operator()(patch, value.as_bool());
    case Value::Type::byteValue:
      return operator()(patch, value.as_byte());
    case Value::Type::i16Value:
      return operator()(patch, value.as_i16());
    case Value::Type::i32Value:
      return operator()(patch, value.as_i32());
    case Value::Type::i64Value:
      return operator()(patch, value.as_i64());
    case Value::Type::floatValue:
      return operator()(patch, value.as_float());
    case Value::Type::doubleValue:
      return operator()(patch, value.as_double());
    case Value::Type::stringValue: {
      auto binaryValue = folly::IOBuf::wrapBufferAsValue(
          value.as_string().data(), value.as_string().size());
      operator()(patch, binaryValue);
      value.emplace_string(binaryValue.to<std::string>());
      return;
    }
    case Value::Type::binaryValue:
      return operator()(patch, value.as_binary());
    case Value::Type::listValue:
      return operator()(patch, value.as_list());
    case Value::Type::setValue:
      return operator()(patch, value.as_set());
    case Value::Type::mapValue:
      return operator()(patch, value.as_map());
    case Value::Type::objectValue:
      return operator()(patch, value.as_object());
    default:
      folly::throw_exception<std::runtime_error>(
          "Not Implemented type support.");
  }
}

void ApplyPatch::operator()(const Object& patch, bool& value) const {
  checkOps(
      patch,
      Value::Type::boolValue,
      {PatchOp::Assign, PatchOp::Put, PatchOp::Clear});
  if (applyAssign<type::bool_t>(patch, value)) {
    return; // Ignore all other ops.
  }
  if (auto* clear = findOp(patch, PatchOp::Clear);
      clear != nullptr && clear->as_bool()) {
    value = false;
  }
  if (auto* invert = findOp(patch, PatchOp::Put)) { // Put is Invert for bool.
    if (argAs<type::bool_t>(*invert)) {
      value = !value;
    }
  }
}

void ApplyPatch::operator()(const Object& patch, int8_t& value) const {
  applyNumericPatch<type::byte_t>(patch, value);
}
void ApplyPatch::operator()(const Object& patch, int16_t& value) const {
  applyNumericPatch<type::i16_t>(patch, value);
}
void ApplyPatch::operator()(const Object& patch, int32_t& value) const {
  applyNumericPatch<type::i32_t>(patch, value);
}
void ApplyPatch::operator()(const Object& patch, int64_t& value) const {
  applyNumericPatch<type::i64_t>(patch, value);
}
void ApplyPatch::operator()(const Object& patch, float& value) const {
  applyNumericPatch<type::float_t>(patch, value);
}
void ApplyPatch::operator()(const Object& patch, double& value) const {
  applyNumericPatch<type::double_t>(patch, value);
}

void ApplyPatch::operator()(const Object& patch, folly::IOBuf& value) const {
  checkOps(
      patch,
      Value::Type::binaryValue,
      {PatchOp::Assign, PatchOp::Clear, PatchOp::Add, PatchOp::Put});
  if (applyAssign<type::cpp_type<folly::IOBuf, type::binary_t>>(patch, value)) {
    return; // Ignore all other ops.
  }

  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (argAs<type::bool_t>(*clear)) {
      value = folly::IOBuf{};
    }
  }

  auto* prepend = findOp(patch, PatchOp::Add);
  auto* append = findOp(patch, PatchOp::Put);
  if (append || prepend) {
    folly::IOBufQueue queue;
    if (prepend) {
      queue.append(argAs<type::binary_t>(*prepend));
    }
    queue.append(value);
    if (append) {
      queue.append(argAs<type::binary_t>(*append));
    }
    value = queue.moveAsValue();
  }
}

void ApplyPatch::operator()(
    const Object& patch, std::vector<Value>& value) const {
  DCHECK(
      !findOp(patch, PatchOp::Add)); // Make sure no one relies on List::prepend
  checkOps(
      patch,
      Value::Type::listValue,
      {PatchOp::Assign, PatchOp::Clear, PatchOp::Put});
  if (applyAssign<type::list_c>(patch, value)) {
    return; // Ignore all other ops.
  }

  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (argAs<type::bool_t>(*clear)) {
      value.clear();
    }
  }

  if (auto* append = findOp(patch, PatchOp::Put)) {
    const auto* appendVector = append->if_list();
    if (!appendVector) {
      throw std::runtime_error("list put patch should contain a list");
    }
    value.insert(value.end(), appendVector->begin(), appendVector->end());
  }
}

void ApplyPatch::operator()(
    const Object& patch, folly::F14FastSet<Value>& value) const {
  checkOps(
      patch,
      Value::Type::setValue,
      {PatchOp::Assign,
       PatchOp::Clear,
       PatchOp::Add,
       PatchOp::Put,
       PatchOp::Remove});
  if (applyAssign<type::set_c>(patch, value)) {
    return; // Ignore all other ops.
  }

  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (argAs<type::bool_t>(*clear)) {
      value.clear();
    }
  }

  auto validate_if_set = [](const auto patchOp, auto name) {
    const auto* opData = patchOp->if_set();
    if (!opData) {
      throw std::runtime_error(
          fmt::format("set {} patch should contain a set", name));
    }
    return opData;
  };

  if (auto* remove = findOp(patch, PatchOp::Remove)) {
    for (const auto& key : *validate_if_set(remove, "remove")) {
      value.erase(key);
    }
  }

  auto insert_set = [&](const auto& to_insert) {
    value.insert(to_insert.begin(), to_insert.end());
  };

  if (auto* add = findOp(patch, PatchOp::Add)) {
    insert_set(*validate_if_set(add, "add"));
  }

  if (auto* put = findOp(patch, PatchOp::Put)) {
    insert_set(*validate_if_set(put, "put"));
  }
}

void ApplyPatch::operator()(
    const Object& patch, folly::F14FastMap<Value, Value>& value) const {
  checkOps(
      patch,
      Value::Type::mapValue,
      {PatchOp::Assign,
       PatchOp::Clear,
       PatchOp::PatchPrior,
       PatchOp::EnsureStruct,
       PatchOp::Put,
       PatchOp::Remove,
       PatchOp::PatchAfter});
  if (applyAssign<type::map_c>(patch, value)) {
    return; // Ignore all other ops.
  }

  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (argAs<type::bool_t>(*clear)) {
      value.clear();
    }
  }

  auto validated_map = [](auto patchOp, auto patchOpName) {
    auto opData = patchOp->if_map();
    if (!opData) {
      throw std::runtime_error(
          fmt::format("map {} patch should contain map", patchOpName));
    }
    return opData;
  };

  auto patchElements = [&](auto patchFields) {
    for (const auto& [keyv, valv] :
         *validated_map(patchFields, "patch/patchPrior")) {
      // Only patch values for fields that exist for now
      if (auto* field = folly::get_ptr(value, keyv)) {
        applyPatch(valv.as_object(), *field);
      }
    }
  };

  if (auto* patchFields = findOp(patch, PatchOp::PatchPrior)) {
    patchElements(patchFields);
  }

  // This is basicly inserting key/value pair into the map if key doesn't exist
  if (auto* ensure = findOp(patch, PatchOp::EnsureStruct)) {
    const auto* mapVal = validated_map(ensure, "ensureStruct");
    value.insert(mapVal->begin(), mapVal->end());
  }

  if (auto* remove = findOp(patch, PatchOp::Remove)) {
    const auto* to_remove = remove->if_set();
    if (!to_remove) {
      throw std::runtime_error("map remove patch should contain set");
    }
    for (const auto& key : *to_remove) {
      value.erase(key);
    }
  }

  if (auto* put = findOp(patch, PatchOp::Put)) {
    for (const auto& [key, val] : *validated_map(put, "put")) {
      value.insert_or_assign(key, val);
    }
  }

  if (auto* patchFields = findOp(patch, PatchOp::PatchAfter)) {
    patchElements(patchFields);
  }
}

template <class Patch>
void impl(Patch&& patch, Object& value) {
  static_assert(std::is_same_v<folly::remove_cvref_t<Patch>, Object>);
  checkOps(
      patch,
      Value::Type::objectValue,
      {PatchOp::Assign,
       PatchOp::Clear,
       PatchOp::Remove,
       PatchOp::PatchPrior,
       PatchOp::EnsureStruct,
       PatchOp::EnsureUnion,
       PatchOp::PatchAfter,
       PatchOp::Add});
  if (applyAssign<type::struct_c>(std::forward<Patch>(patch), value)) {
    return; // Ignore all other ops.
  }

  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (argAs<type::bool_t>(*clear)) {
      value.members()->clear();
    }
  }

  auto applyFieldPatch = [&](auto patchFields) {
    const auto* obj = patchFields->if_object();
    if (!obj) {
      throw std::runtime_error(
          "struct patch PatchPrior/Patch should contain an object");
    }
    for (const auto& [id, field_value] : *obj->members()) {
      // Only patch values for fields that exist for now
      if (auto* field = folly::get_ptr(*value.members(), id)) {
        applyPatch(field_value.as_object(), *field);
      }
    }
  };

  if (auto* patchFields = findOp(patch, PatchOp::PatchPrior)) {
    applyFieldPatch(patchFields);
  }

  if (const auto* sensure = findOp(patch, PatchOp::EnsureStruct)) {
    if (const auto* obj = sensure->if_object()) {
      value.members()->insert(obj->begin(), obj->end());
    } else {
      throw std::runtime_error("struct patch Ensure should contain an object");
    }
  }

  if (const auto* uensure = findOp(patch, PatchOp::EnsureUnion)) {
    const auto* ensureUnion = uensure->if_object();
    if (!ensureUnion) {
      throw std::runtime_error("union patch Ensure should contain an object");
    }

    // EnsureUnion is not optional and will contain an empty union by default.
    // We should ignore such cases.
    if (!ensureUnion->empty()) {
      if (ensureUnion->size() != 1) {
        throw std::runtime_error(
            "union patch Ensure should contain an object with only one field set");
      }

      auto& id = ensureUnion->begin()->first;
      auto itr = value.members()->find(id);
      if (itr == value.end()) {
        value = *ensureUnion;
      } else if (value.size() != 1) {
        // Clear other values, without copying the current value
        *value.members() = {{itr->first, std::move(itr->second)}};
      }
    }
  }

  if (auto* patchFields = findOp(patch, PatchOp::PatchAfter)) {
    applyFieldPatch(patchFields);
  }
  if (auto* to_remove = findOp(patch, PatchOp::Remove)) {
    auto remove = [&](const auto& ids) {
      for (const auto& field_id : ids) {
        if (!field_id.is_i16()) {
          throw std::runtime_error(fmt::format(
              "The `PatchOp::Remove` field in struct/union patch is not `set<i16>` but `set<{}>`",
              util::enumNameSafe(field_id.getType())));
        }

        value.erase(FieldId{field_id.as_i16()});
      }
    };

    if (const auto* p_set = to_remove->if_set()) {
      // TODO: Remove this after migrating to List
      remove(*p_set);
    } else if (const auto* p_list = to_remove->if_list()) {
      remove(*p_list);
    } else {
      throw std::runtime_error(fmt::format(
          "The `PatchOp::Remove` field in struct/union patch is not `set<i16>`/`list<i16>` but `{}`",
          util::enumNameSafe(to_remove->getType())));
    }
  }
  if (auto* addFields = findOp(patch, PatchOp::Add)) {
    // TODO(afuller): Implement field-wise add.
  }
}

void ApplyPatch::operator()(const Object& patch, Object& value) const {
  impl(patch, value);
}
void ApplyPatch::operator()(Object&& patch, Object& value) const {
  impl(std::move(patch), value);
}

// Inserts the next mask with union operator to getIncludesRef(mask)[id].
// Skips if mask is allMask (already includes all fields), or next is noneMask.
template <typename Id, typename F>
void insertMaskUnion(
    Mask& mask, Id id, const Mask& next, const F& getIncludesRef) {
  if (mask != allMask() && next != noneMask()) {
    Mask& current = getIncludesRef(mask)
                        .ensure()
                        .emplace(std::move(id), noneMask())
                        .first->second;
    current = current | next;
  }
}

// Insert allMask to getIncludesRef(mask)[id] if id does not exist.
template <typename Id, typename F>
void tryInsertAllMask(Mask& mask, Id id, const F& getIncludesRef) {
  getIncludesRef(mask).ensure().emplace(std::move(id), allMask());
}

template <typename Id, typename F>
void insertNextMask(
    ExtractedMasksFromPatch& masks,
    const Value& nextPatch,
    Id readId,
    Id writeId,
    bool recursive,
    bool view,
    const F& getIncludesRef) {
  if (recursive) {
    auto nextMasks = view
        ? protocol::extractMaskViewFromPatch(nextPatch.as_object())
        : protocol::extractMaskFromPatch(nextPatch.as_object());
    insertMaskUnion(
        masks.read, std::move(readId), nextMasks.read, getIncludesRef);
    insertMaskUnion(
        masks.write, std::move(writeId), nextMasks.write, getIncludesRef);
  } else {
    insertMaskUnion(masks.read, std::move(readId), allMask(), getIncludesRef);
    insertMaskUnion(masks.write, std::move(writeId), allMask(), getIncludesRef);
  }
}

// Ensure requires reading existing value to know whether the field is set or
// not. Insert allMask() if the field was never included in read mask
// before.
void insertEnsureReadFieldsToMask(Mask& mask, const Value& ensureFields) {
  const auto& obj = ensureFields.as_object();
  auto getIncludesObjRef = [&](Mask& m) { return m.includes_ref(); };
  for (const auto& [id, value] : obj) {
    tryInsertAllMask(mask, id, getIncludesObjRef);
  }
}

// Ensure only requires writing fields that exists in the patch. Put allMask()
// iff current mask is not allMask and the field was never included in the write
// mask before.
void insertEnsureWriteFieldsToMask(Mask& mask, const Value& ensureFields) {
  if (mask == allMask()) {
    return;
  }
  const auto& obj = ensureFields.as_object();
  for (const auto& [id, value] : obj) {
    mask.includes_ref().ensure().emplace(id, allMask());
  }
}

// Ensure requires reading existing value to know whether keys exist in the
// map. Insert allMask() if the key was never included in read mask
// before.
void insertEnsureReadKeysToMask(
    Mask& mask, const Value& ensureFields, bool view) {
  const auto& map = ensureFields.as_map();
  auto getIncludesMapRef = [&](Mask& mask) { return mask.includes_map_ref(); };
  auto getIncludesStringMapRef = [&](Mask& mask) {
    return mask.includes_string_map_ref();
  };
  if (view) {
    auto readValueIndex = buildValueIndex(mask);
    for (const auto& [key, value] : map) {
      auto id = static_cast<int64_t>(
          getMapIdValueAddressFromIndex(readValueIndex, key));
      tryInsertAllMask(mask, id, getIncludesMapRef);
    }
    return;
  }
  for (const auto& [key, value] : map) {
    if (getArrayKeyFromValue(key) == ArrayKey::Integer) {
      tryInsertAllMask(
          mask,
          static_cast<int64_t>(getMapIdFromValue(key)),
          getIncludesMapRef);
    } else {
      tryInsertAllMask(mask, getStringFromValue(key), getIncludesStringMapRef);
    }
  }
}

template <typename T>
const T& getKeyOrElem(const T& value) {
  return value;
}
template <typename K, typename V>
const K& getKeyOrElem(const std::pair<const K, V>& value) {
  return value.first;
}

// Put allMask() iff current mask is not allMask and the key was never included
// in the write mask before. `view` specifies whether to use address of Value to
// populate map mask (deprecated).
template <typename Container>
void insertWriteKeysToMapMask(Mask& mask, const Container& c, bool view) {
  if (mask == allMask()) {
    return;
  }

  if (view) {
    auto writeValueIndex = buildValueIndex(mask);
    for (const auto& elem : c) {
      const auto& v = getKeyOrElem(elem);
      auto id = static_cast<int64_t>(
          getMapIdValueAddressFromIndex(writeValueIndex, v));
      mask.includes_map_ref().ensure().emplace(id, allMask());
    }
    return;
  }

  for (const auto& elem : c) {
    const auto& v = getKeyOrElem(elem);

    if (getArrayKeyFromValue(v) == ArrayKey::Integer) {
      mask.includes_map_ref().ensure().emplace(
          static_cast<int64_t>(getMapIdFromValue(v)), allMask());
    } else {
      mask.includes_string_map_ref().ensure().emplace(
          getStringFromValue(v), allMask());
    }
  }
}

// Remove only requires writing fields that exists in the patch.
void insertRemoveWriteFieldsToMask(Mask& mask, const std::vector<Value>& ids) {
  if (mask == allMask()) {
    return;
  }
  for (const Value& id : ids) {
    mask.includes_ref().ensure().emplace(id.as_i16(), allMask());
  }
}

// If recursive, it constructs the mask from the patch object for the field.
// If view, it uses address of Value to populate map mask. If not view, it
// uses the appropriate integer map mask and string map mask after parsing
// from Value.
void insertFieldsToMask(
    ExtractedMasksFromPatch& masks,
    const Value& patchFields,
    bool recursive,
    bool view) {
  auto getIncludesMapRef = [&](Mask& mask) { return mask.includes_map_ref(); };
  auto getIncludesStringMapRef = [&](Mask& mask) {
    return mask.includes_string_map_ref();
  };

  if (const auto* obj = patchFields.if_object()) {
    auto getIncludesObjRef = [&](Mask& mask) { return mask.includes_ref(); };
    for (const auto& [id, value] : *obj) {
      // Object patch can get here only StructPatch::Patch(Prior|After
      // operations, which require reading existing value to know if/how given
      // operations can/should be applied. Generate allMask() read mask for them
      // if the recursively extracted masks from patch does not include the
      // field.
      insertNextMask(masks, value, id, id, recursive, view, getIncludesObjRef);
      tryInsertAllMask(masks.read, id, getIncludesObjRef);
    }
  } else if (const auto* map = patchFields.if_map()) {
    // Map patch can get here only MapPatch::Patch(Prior|After) operations,
    // which require reading existing value to know if/how given operations
    // can/should be applied. Generate allMask() read map mask if the
    // recursively extracted masks from patch do not include the key.
    if (view) {
      auto readValueIndex = buildValueIndex(masks.read);
      auto writeValueIndex = buildValueIndex(masks.write);
      for (const auto& [key, value] : *map) {
        auto readId = static_cast<int64_t>(
            getMapIdValueAddressFromIndex(readValueIndex, key));
        auto writeId = static_cast<int64_t>(
            getMapIdValueAddressFromIndex(writeValueIndex, key));
        insertNextMask(
            masks, value, readId, writeId, recursive, view, getIncludesMapRef);
        tryInsertAllMask(masks.read, readId, getIncludesMapRef);
      }
      return;
    }
    for (const auto& [key, value] : *map) {
      if (getArrayKeyFromValue(key) == ArrayKey::Integer) {
        auto id = static_cast<int64_t>(getMapIdFromValue(key));
        insertNextMask(
            masks, value, id, id, recursive, view, getIncludesMapRef);
        tryInsertAllMask(masks.read, id, getIncludesMapRef);
      } else {
        auto id = getStringFromValue(key);
        insertNextMask(
            masks, value, id, id, recursive, view, getIncludesStringMapRef);
        tryInsertAllMask(masks.read, id, getIncludesStringMapRef);
      }
    }
  }
}

ExtractedMasksFromPatch extractMaskFromPatch(
    const protocol::Object& patch, bool view) {
  ExtractedMasksFromPatch masks = {noneMask(), noneMask()};
  // If Assign, it is a write operation
  if (findOp(patch, PatchOp::Assign)) {
    return {noneMask(), allMask()};
  }
  // If Clear, it is a write operation if not intristic default.
  if (auto* clear = findOp(patch, PatchOp::Clear)) {
    if (!isIntrinsicDefault(*clear)) {
      masks = {noneMask(), allMask()};
    }
  }
  // If Add, it is a read-write operation if not intristic default.
  if (auto* add = findOp(patch, PatchOp::Add)) {
    if (!isIntrinsicDefault(*add)) {
      return {allMask(), allMask()};
    }
  }

  // If Put, it is a write operation for map and read-write operation for
  // others.
  if (auto* value = findOp(patch, PatchOp::Put)) {
    if (value->is_map()) {
      insertWriteKeysToMapMask(masks.write, value->as_map(), view);
    } else if (!isIntrinsicDefault(*value)) {
      return {allMask(), allMask()};
    }
  }
  // We can only distinguish struct. For struct, add removed fields to write
  // mask. Both set and map use a set for Remove, so they are indistinguishable.
  // For set and map, we cannot distinguish them. For view, we always add
  // removed keys to write map mask. For non-view, it is a read-write operation
  // if not intristic default.
  if (auto* value = findOp(patch, PatchOp::Remove)) {
    if (value->is_list()) {
      // struct patch
      insertRemoveWriteFieldsToMask(masks.write, value->as_list());
    } else if (!isIntrinsicDefault(*value)) {
      // set/map patch
      if (!view) {
        return {allMask(), allMask()};
      }
      insertWriteKeysToMapMask(masks.write, value->as_set(), view);
    }
  }

  // If PatchPrior or PatchAfter, recursively constructs the mask for the
  // fields.
  for (auto op : {PatchOp::PatchPrior, PatchOp::PatchAfter}) {
    if (auto* patchFields = findOp(patch, op)) {
      insertFieldsToMask(masks, *patchFields, true, view);
    }
  }

  // If EnsureStruct, add fields/keys to mask.
  if (auto* ensureStruct = findOp(patch, PatchOp::EnsureStruct)) {
    if (ensureStruct->if_object()) {
      insertEnsureReadFieldsToMask(masks.read, *ensureStruct);
      insertEnsureWriteFieldsToMask(masks.write, *ensureStruct);
    } else {
      insertEnsureReadKeysToMask(masks.read, *ensureStruct, view);
      insertWriteKeysToMapMask(masks.write, ensureStruct->as_map(), view);
    }
  }

  // If EnsureUnion, add fields to mask for read mask and all mask for write
  // mask.
  if (auto* ensureUnion = findOp(patch, PatchOp::EnsureUnion)) {
    insertEnsureReadFieldsToMask(masks.read, *ensureUnion);
    masks.write = allMask();
  }

  // Read mask should be always subset of write mask. If not, make read mask
  // equal to write mask. This can happen for struct or map fields with patch
  // operations that returns noneMask for read mask (i.e. assign).
  if ((masks.read | masks.write) != masks.write) {
    masks.read = masks.write;
  }

  return masks;
}

} // namespace detail

ExtractedMasksFromPatch extractMaskViewFromPatch(
    const protocol::Object& patch) {
  detail::checkNotSafePatch(patch);
  return detail::extractMaskFromPatch(patch, true);
}

ExtractedMasksFromPatch extractMaskFromPatch(const protocol::Object& patch) {
  detail::checkNotSafePatch(patch);
  return detail::extractMaskFromPatch(patch, false);
}

template <type::StandardProtocol Protocol>
std::unique_ptr<folly::IOBuf> applyPatchToSerializedData(
    const protocol::Object& patch, const folly::IOBuf& buf) {
  detail::checkNotSafePatch(patch);
  // TODO: create method for this operation
  static_assert(
      Protocol == type::StandardProtocol::Binary ||
      Protocol == type::StandardProtocol::Compact);
  using ProtocolReader = std::conditional_t<
      Protocol == type::StandardProtocol::Binary,
      BinaryProtocolReader,
      CompactProtocolReader>;
  using ProtocolWriter = std::conditional_t<
      Protocol == type::StandardProtocol::Binary,
      BinaryProtocolWriter,
      CompactProtocolWriter>;
  auto masks = protocol::extractMaskViewFromPatch(patch);
  MaskedDecodeResult result =
      parseObject<ProtocolReader>(buf, masks.read, masks.write);
  applyPatch(patch, result.included);
  return serializeObject<ProtocolWriter>(result.included, result.excluded);
}

// Uses explicit instantiations to have the function definition in .cpp file.
template std::unique_ptr<folly::IOBuf>
applyPatchToSerializedData<type::StandardProtocol::Binary>(
    const protocol::Object& patch, const folly::IOBuf& buf);
template std::unique_ptr<folly::IOBuf>
applyPatchToSerializedData<type::StandardProtocol::Compact>(
    const protocol::Object& patch, const folly::IOBuf& buf);

Object fromSafePatch(const protocol::Object& safePatch) {
  const Value* version = safePatch.if_contains(detail::kSafePatchVersionId);
  const Value* data = safePatch.if_contains(detail::kSafePatchDataId);
  if (!(version && version->is_i32() && data && data->is_binary())) {
    throw std::runtime_error("Invalid safe patch");
  }
  if (version->as_i32() == 0) {
    throw std::runtime_error("Invalid safe patch");
  }
  if (version->as_i32() > op::detail::kThriftDynamicPatchVersion) {
    throw std::runtime_error(
        fmt::format("Unsupported patch version: {}", version->as_i32()));
  }
  Object patch =
      parseObject<CompactProtocolReader>(safePatch[FieldId{2}].as_binary());
  return patch;
}

Object toSafePatch(const protocol::Object& patch) {
  Object safePatch;
  safePatch[detail::kSafePatchVersionId].emplace_i32(
      op::detail::kThriftDynamicPatchVersion);
  safePatch[detail::kSafePatchDataId].emplace_binary(
      *serializeObject<CompactProtocolWriter>(patch));
  return safePatch;
}

} // namespace protocol
} // namespace thrift
} // namespace apache
