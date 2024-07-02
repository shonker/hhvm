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

#include <utility>

#include <folly/Traits.h>
#include <thrift/lib/cpp2/op/Clear.h>
#include <thrift/lib/cpp2/op/Create.h>
#include <thrift/lib/cpp2/op/Get.h>
#include <thrift/lib/cpp2/op/detail/BasePatch.h>
#include <thrift/lib/cpp2/type/Field.h>
#include <thrift/lib/cpp2/type/Id.h>
#include <thrift/lib/cpp2/type/NativeType.h>

namespace apache {
namespace thrift {
namespace op {
namespace detail {

struct FieldIdListToSetAdapter {
  using FieldIdSet = std::unordered_set<FieldId>;
  using FieldIdList = std::vector<std::int16_t>;
  static FieldIdSet fromThrift(const FieldIdList& v) {
    FieldIdSet ret;
    ret.reserve(v.size());
    for (auto i : v) {
      ret.emplace(static_cast<FieldId>(i));
    }
    return ret;
  }
  static FieldIdList toThrift(const FieldIdSet& v) {
    FieldIdList ret;
    ret.reserve(v.size());
    for (auto i : v) {
      ret.emplace_back(folly::to_underlying(i));
    }
    return ret;
  }
};

/// Patch for a Thrift field.
///
/// Requires Patch have fields with ids 1:1 with the fields they patch.
template <typename Patch>
class FieldPatch : public BasePatch<Patch, FieldPatch<Patch>> {
  using Base = BasePatch<Patch, FieldPatch>;

 public:
  using Base::apply;
  using Base::Base;
  using Base::operator=;
  using Base::get;

  template <typename T>
  static FieldPatch createFrom(T&& val) {
    FieldPatch patch;
    patch.assignFrom(std::forward<T>(val));
    return patch;
  }

  /// Returns the pointer to the Thrift patch struct.
  Patch* operator->() noexcept { return &data_; }
  const Patch* operator->() const noexcept { return &data_; }
  /// Returns the reference to the Thrift patch struct.
  Patch& operator*() noexcept { return data_; }
  const Patch& operator*() const noexcept { return data_; }

  template <typename Visitor>
  void customVisit(Visitor&& v) const {
    for_each_field_id<Patch>(
        [&](auto id) { v.template patchIfSet<decltype(id)>(*get(id)); });
  }

 private:
  using Base::data_;
};

/// Create a base patch that supports Ensure operator.
///
/// The `Patch` template parameter must be a Thrift struct with the following
/// fields:
/// * `optional T assign`
/// * `terse bool clear`
/// * `terse P patchPrior`
/// * `terse T ensure`
/// * `terse P patch`
/// Where `P` is the field patch type for the struct type `T`.
template <typename Patch, typename Derived>
class BaseEnsurePatch : public BaseClearPatch<Patch, Derived> {
  using Base = BaseClearPatch<Patch, Derived>;
  using T = typename Base::value_type;
  template <typename Id>
  using FieldType = type::native_type<get_field_tag<T, Id>>;

  // Needed to access patchIfSet(...) for merge(...) method
  template <class>
  friend class FieldPatch;

  struct Applier {
    T& v;

    void assign(const T& t) { v = t; }
    void clear() { ::apache::thrift::clear(v); }

    template <class Id, class FieldPatch>
    void patchIfSet(const FieldPatch& patch) {
      patch.apply(op::get<Id>(v));
    }

    template <class>
    void ensure() {
      // For non-optional field, ensure is no-op
    }

    template <class Id, class Field>
    void ensure(const Field& def) {
      if (isAbsent(op::get<Id>(v))) {
        op::ensure<Id>(v) = def;
      }
    }

    template <class Id>
    void remove() {
      using field_tag = op::get_field_tag<T, Id>;
      op::clear_field<field_tag>(op::get<Id>(v), v);
    }
  };

  template <typename Id>
  void removeImpl() {
    ensurePatchable();
    Base::toThrift().remove()->insert(op::get_field_id_v<T, Id>);
  }

  template <typename Id>
  std::enable_if_t<!type::is_optional_or_union_field_v<T, Id>> remove() {
    // Usually for non-optional field, it should not be removable.
    // This can only happen if the field was optional when creating the Patch,
    // but later we changed the field to non-optional.
    // In this case we should set it to intrinsic default when removing it.
    removeImpl<Id>();
  }

 public:
  using Base::Base;
  using Base::operator=;
  using Base::assign;
  /// Corresponding FieldPatch of this struct patch.
  using patch_type = get_native_type<Patch, ident::patch>;

  BaseEnsurePatch(const BaseEnsurePatch&) = default;
  BaseEnsurePatch(BaseEnsurePatch&&) noexcept = default;
  BaseEnsurePatch& operator=(const BaseEnsurePatch&) = default;
  BaseEnsurePatch& operator=(BaseEnsurePatch&&) noexcept = default;

  /// Returns if the patch ensures the given field is set (explicitly or
  /// implicitly).
  template <typename Id>
  constexpr bool ensures() const {
    return !isAbsent(getEnsure<Id>(data_));
  }

  bool empty() const {
    bool b = true;
    op::for_each_ordinal<T>([&](auto id) {
      using Id = decltype(id);
      if constexpr (!apache::thrift::detail::is_shared_ptr_v<
                        op::get_field_ref<T, Id>>) {
        b = b && !this->modifies<Id>();
      }
    });
    return b;
  }

  template <typename Id>
  std::enable_if_t<
      type::is_optional_or_union_field_v<T, Id> && !is_thrift_union_v<T>>
  remove() {
    removeImpl<Id>();
  }

  /// Returns if the patch modifies the given field.
  template <typename Id>
  bool modifies() const {
    return hasAssign() || data_.clear() == true ||
        (getEnsure<Id>(data_) && type::is_optional_or_union_field_v<T, Id>) ||
        !getRawPatch<Id>(data_.patchPrior()).empty() ||
        !getRawPatch<Id>(data_.patch()).empty();
  }

  template <typename Id, typename... Ids>
  std::enable_if_t<sizeof...(Ids) != 0, bool> modifies() const {
    // If hasAssign() == true, the whole struct (all fields) will be replaced.
    if (hasAssign() || data_.clear() == true ||
        (getEnsure<Id>(data_) && type::is_optional_or_union_field_v<T, Id>)) {
      return true;
    }

    return getRawPatch<Id>(data_.patchPrior()).template modifies<Ids...>() ||
        getRawPatch<Id>(data_.patch()).template modifies<Ids...>();
  }

  /// Ensures the given field is set.
  template <typename Id>
  void ensure() {
    // Ensuring non-optional field to intrinsic default is allowed since we
    // might want to ensure field in case the field doesn't exist in dynamic
    // value. (e.g., Terse field with default value. Without ensuring it first,
    // we will not be able to patch such field at all).
    maybeEnsure<Id>();
  }
  /// Same as `ensure()` method, except uses the provided default value.
  template <typename Id, typename U = FieldType<Id>>
  std::enable_if_t<type::is_optional_or_union_field_v<T, Id>> ensure(
      U&& defaultVal) {
    if (maybeEnsure<Id>()) {
      if (patchPrior<Id>().toThrift().clear().value() &&
          !is_thrift_union_v<T>) {
        // If the field is cleared, we need to assign the value in PatchAfter.
        // Why? In dynamic patch, the PatchOp::Clear in PatchPrior will set the
        // field to intrinsic default rather than removing it (unlike static
        // patch). If we assign the value in Patch after, then the behavior
        // would be the same between static/dynamic patch.
        patchPrior<Id>().reset();
        patchAfter<Id>() = std::forward<U>(defaultVal);
      } else {
        getEnsure<Id>(data_) = std::forward<U>(defaultVal);
      }
    }
  }
  /// Ensures the given field is initalized, and return the associated patch
  /// object.
  template <typename Id>
  decltype(auto) patch() {
    return (maybeEnsure<Id>(), patchAfter<Id>());
  }

  /// Returns the proper patch object for the given field.
  template <typename Id>
  decltype(auto) patchIfSet() {
    if (!type::is_optional_or_union_field_v<T, Id>) {
      return patch<Id>();
    }
    ensurePatchable();
    if constexpr (!is_thrift_union_v<T>) {
      if (Base::derived().template isRemoved<Id>()) {
        // If field is already cleared, Patch should be ignored.
        getRawPatch<Id>(data_.patch()).toThrift().clear() = true;
        return getRawPatch<Id>(data_.patchPrior());
      }
    }
    return ensures<Id>() ? getRawPatch<Id>(data_.patch())
                         : getRawPatch<Id>(data_.patchPrior());
  }

  /// @copybrief AssignPatch::customVisit
  ///
  /// Users should provide a visitor with the following methods
  ///
  ///     struct Visitor {
  ///       void assign(const MyClass&);
  ///       void clear();
  ///       template<class Id> void patchIfSet(const FieldPatch&);
  ///       // For optional fields in structs and fields in unions
  ///       template<class Id> void ensure(const op::get_native_type<Id>&);
  ///       // For non-optional fields in structs
  ///       template<class Id> void ensure();
  ///     }
  ///
  /// For example, let's assume you have the following thrift struct:
  ///
  ///     struct MyClass {
  ///       1: string foo;
  ///       2: bool bar;
  ///     }
  ///
  /// and then you created the following patch:
  ///
  ///     MyClassPatch patch;
  ///     patch.patch<ident::bar>().invert();
  ///     patch.patch<ident::bar>().invert();
  ///     patch.patch<ident::foo>().append("_");
  ///
  /// `patch.customVisit(v)` will invoke the following methods
  ///
  ///     v.ensure<ident::foo>();
  ///     v.ensure<ident::bar>();
  ///     v.patchIfSet<ident::foo>(StringPatch::createAppend("_"));
  ///     v.patchIfSet<ident::bar>(BoolPatch{});  // no-op since inverted twice
  template <typename Visitor>
  void customVisit(Visitor&& v) const {
    if (false) {
      // Test whether the required methods exist in Visitor
      v.assign(T{});
      v.clear();
      for_each_field_id<T>([&](auto id) {
        using Id = decltype(id);
        if constexpr (!apache::thrift::detail::is_shared_ptr_v<
                          op::get_field_ref<T, Id>>) {
          using FieldPatchType =
              folly::remove_cvref_t<decltype(BaseEnsurePatch{}.patch<Id>())>;
          v.template patchIfSet<Id>(FieldPatchType{});
          v.template ensure<Id>();
          if constexpr (type::is_optional_or_union_field_v<T, Id>) {
            v.template ensure<Id>(FieldType<Id>{});
          }
        }
      });
    }

    if (Base::template customVisitAssignAndClear(std::forward<Visitor>(v))) {
      return;
    }

    data_.patchPrior()->customVisit(std::forward<Visitor>(v));

    // TODO: Optimize ensure for UnionPatch
    for_each_field_id<T>([&](auto id) {
      using Id = decltype(id);
      if constexpr (!apache::thrift::detail::is_shared_ptr_v<
                        op::get_field_ref<T, Id>>) {
        if (auto p = op::get<Id>(*data_.ensure())) {
          if constexpr (type::is_optional_or_union_field_v<T, Id>) {
            std::forward<Visitor>(v).template ensure<Id>(*p);
          } else {
            std::forward<Visitor>(v).template ensure<Id>();
          }
        }
      }
    });

    data_.patch()->customVisit(std::forward<Visitor>(v));

    if constexpr (!is_thrift_union_v<T>) {
      for (auto fieldId : *data_.remove()) {
        op::invoke_by_field_id<T>(
            fieldId,
            [&](auto id) {
              using Id = decltype(id);
              if constexpr (!apache::thrift::detail::is_shared_ptr_v<
                                op::get_field_ref<T, Id>>) {
                std::forward<Visitor>(v).template remove<Id>();
              }
            },
            [] {
              // Ignore if the specified field is not part of the struct
              // considering schema evolution.
            });
      }
    }
  }

  void apply(T& val) const { return customVisit(Applier{val}); }

 protected:
  using Base::apply;
  using Base::data_;
  using Base::hasAssign;
  ~BaseEnsurePatch() = default;

  // Clears the field with the given id.
  template <typename Id>
  void clear() {
    if (hasAssign()) {
      op::clear<Id>(*data_.assign());
      return;
    }
    patchPrior<Id>().clear();
    op::clear<Id>(*data_.ensure());
    patchAfter<Id>().reset();
  }
  using Base::clear;

  template <typename Id, typename U>
  static decltype(auto) getEnsure(U&& data) {
    return op::get<Id>(*std::forward<U>(data).ensure());
  }

  template <typename Id>
  decltype(auto) patchPrior() {
    return (ensurePatchable(), getRawPatch<Id>(data_.patchPrior()));
  }

  template <typename Id>
  decltype(auto) patchAfter() {
    return (ensurePatchable(), getRawPatch<Id>(data_.patch()));
  }

  void ensurePatchable() {
    if (data_.assign().has_value()) {
      for_each_field_id<T>([&](auto id) {
        using Id = decltype(id);
        if constexpr (!apache::thrift::detail::is_shared_ptr_v<
                          op::get_field_ref<T, Id>>) {
          auto&& field = op::get<>(id, *data_.assign());
          auto&& prior = getRawPatch<Id>(data_.patchPrior());
          auto&& ensure = op::get<>(id, *data_.ensure());
          auto&& after = getRawPatch<Id>(data_.patch());
          if (isAbsent(field)) {
            prior.toThrift().clear() = true;
          } else {
            ensure = {};
            after.assign(std::move(*field));
          }
        }
      });
      // Unset assign.
      data_.assign().reset();
    }
  }

  template <typename Id>
  bool maybeEnsure() {
    if (*patchAfter<Id>().toThrift().clear()) {
      // Since we cleared the field in PatchAfter, we should remove any existing
      // ensured value.
      op::clear<Id>(*data_.ensure());
    }

    if constexpr (!is_thrift_union_v<T>) {
      auto removeRef = Base::toThrift().remove();
      auto iter = removeRef->find(op::get_field_id_v<T, Id>);
      if (iter != removeRef->end()) {
        // If the current patch removed the field and now we want to ensure it,
        // we should clear it in patch prior and then ensure it.
        removeRef->erase(iter);
        patchPrior<Id>().reset();
        patchAfter<Id>().reset();
        patchPrior<Id>().toThrift().clear() = true;
        getEnsure<Id>(data_).emplace();
        return true;
      }
    }

    if (ensures<Id>()) {
      return false;
    }
    // Merge anything (oddly) in patchAfter into patchPrior.
    if (!patchAfter<Id>().empty()) {
      patchPrior<Id>().merge(std::move(patchAfter<Id>()));
      patchAfter<Id>().reset();
    }
    getEnsure<Id>(data_).ensure();
    return true;
  }

 private:
  template <typename Id, typename U>
  decltype(auto) getRawPatch(U&& patch) const {
    // Field Ids must always be used to access patch(Prior).
    return *patch->get(get_field_id<T, Id>{});
  }

  // Needed for merge(...). We can consider making this a public API.
  template <typename Id, typename FieldPatch>
  void patchIfSet(const FieldPatch& p) {
    patchIfSet<Id>().merge(p);
  }
};

/// Patch for a Thrift struct.
///
/// The `Patch` template parameter must be a Thrift struct with the following
/// fields:
/// * `optional T assign`
/// * `terse bool clear`
/// * `terse P patchPrior`
/// * `terse T ensure`
/// * `terse P patch`
/// Where `P` is the field patch type for the struct type `T`.
template <typename Patch>
class StructPatch : public BaseEnsurePatch<Patch, StructPatch<Patch>> {
  using Base = BaseEnsurePatch<Patch, StructPatch>;
  using T = typename Base::value_type;
  template <typename Id>
  using F = type::native_type<get_field_tag<T, Id>>;

  friend class BaseEnsurePatch<Patch, StructPatch<Patch>>;

 public:
  using Base::apply;
  using Base::assign;
  using Base::Base;
  using Base::operator=;
  using patch_type = get_native_type<Patch, ident::patch>;

  void clear() {
    Base::clear();
    // Custom defaults must also be cleared.
    op::clear<>(*data_.ensure());
  }
  template <typename Id>
  void clear() {
    Base::template clear<Id>();
  }

  /// Assigns to the given field, ensuring first if needed.
  template <typename Id, typename U = F<Id>>
  void assign(U&& val) {
    if (data_.assign().has_value()) {
      op::get<Id>(*data_.assign()) = std::forward<U>(val);
    } else {
      Base::template patch<Id>().assign(std::forward<U>(val));
    }
  }

  template <class Protocol>
  uint32_t encode(Protocol& prot) const {
    // PatchOp::Remove
    using PatchOpRemoveId = field_id<7>;
    uint32_t s = 0;
    s += prot.writeStructBegin(op::get_class_name_v<Patch>.data());
    const auto remove = removedFields();
    op::for_each_field_id<Patch>([&](auto id) {
      using Id = decltype(id);
      using Tag = op::get_type_tag<Patch, Id>;
      constexpr bool isRemoveField = std::is_same<Id, PatchOpRemoveId>::value;

      auto&& field = op::get<Id>(data_);

      if (!isRemoveField && !should_write<Tag>(field)) {
        return;
      }

      if (isRemoveField && remove.empty()) {
        return;
      }

      s += prot.writeFieldBegin(
          &*op::get_name_v<Patch, Id>.begin(),
          typeTagToTType<Tag>,
          folly::to_underlying(Id::value));
      if constexpr (isRemoveField) {
        s += op::encode<Tag>(prot, remove);
      } else {
        s += op::encode<Tag>(prot, *field);
      }
      s += prot.writeFieldEnd();
    });
    s += prot.writeFieldStop();
    s += prot.writeStructEnd();
    return s;
  }

 private:
  using Base::data_;

  // Whether the field is removed
  template <class Id>
  bool isRemoved() const {
    const auto& prior = data_.patchPrior()->toThrift();
    const auto& ensure = *data_.ensure();
    const auto& after = data_.patch()->toThrift();
    const auto& remove = *data_.remove();

    if constexpr (!type::is_optional_or_union_field_v<T, Id>) {
      // non-optional fields can not be removed
      return false;
    }

    if (*get<Id>(after)->toThrift().clear()) {
      // Cleared field in patch after
      return true;
    }

    if (*get<Id>(prior)->toThrift().clear() && !get<Id>(ensure).has_value()) {
      // Cleared field in patch prior and not ensured
      return true;
    }

    return remove.count(op::get_field_id_v<T, Id>);
  }

  // Combine fields from PatchOp::Clear and PatchOp::Remove operations
  std::unordered_set<FieldId> removedFields() const {
    auto removed = *data_.remove();
    op::for_each_field_id<T>([&](auto id) {
      using Id = decltype(id);
      if constexpr (!apache::thrift::detail::is_shared_ptr_v<
                        op::get_field_ref<T, Id>>) {
        if (this->isRemoved<Id>()) {
          removed.insert(id.value);
        }
      }
    });
    return removed;
  }
};

/// Patch for a Thrift union.
///
/// The `Patch` template parameter must be a Thrift struct with the following
/// fields:
/// * `optional T assign`
/// * `terse bool clear`
/// * `terse P patchPrior`
/// * `terse T ensure`
/// * `terse P patch`
/// Where `P` is the field patch type for the union type `T`.
template <typename Patch>
class UnionPatch : public BaseEnsurePatch<Patch, UnionPatch<Patch>> {
  using Base = BaseEnsurePatch<Patch, UnionPatch>;
  using T = typename Base::value_type;
  template <typename Id>
  using F = type::native_type<get_field_tag<T, Id>>;

 public:
  using Base::Base;
  using Base::operator=;
  using Base::apply;
  using Base::assign;
  using Base::clear;

  /// Assigns to the given field, ensuring first if needed.
  template <typename Id, typename U = F<Id>>
  void assign(U&& val) {
    op::get<Id>(Base::resetAnd().assign().ensure()) = std::forward<U>(val);
  }
};

} // namespace detail
} // namespace op
} // namespace thrift
} // namespace apache
