/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/refs/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include "thrift/compiler/test/fixtures/refs/gen-cpp2/module_metadata.h"
#include <thrift/lib/cpp2/visitation/for_each.h>

namespace apache {
namespace thrift {
namespace detail {

template <>
struct ForEachField<::cpp2::MyUnion> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).anInteger_ref()...);
    f(1, static_cast<T&&>(t).aString_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::MyField> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).opt_value_ref()...);
    f(1, static_cast<T&&>(t).value_ref()...);
    f(2, static_cast<T&&>(t).req_value_ref()...);
    f(3, static_cast<T&&>(t).opt_enum_value_ref()...);
    f(4, static_cast<T&&>(t).enum_value_ref()...);
    f(5, static_cast<T&&>(t).req_enum_value_ref()...);
    f(6, static_cast<T&&>(t).opt_str_value_ref()...);
    f(7, static_cast<T&&>(t).str_value_ref()...);
    f(8, static_cast<T&&>(t).req_str_value_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::MyStruct> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).opt_ref_ref()...);
    f(1, static_cast<T&&>(t).ref_ref()...);
    f(2, static_cast<T&&>(t).req_ref_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithUnion> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).u_ref()...);
    f(1, static_cast<T&&>(t).aDouble_ref()...);
    f(2, static_cast<T&&>(t).f_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::RecursiveStruct> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).mes_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithContainers> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).list_ref_ref()...);
    f(1, static_cast<T&&>(t).set_ref_ref()...);
    f(2, static_cast<T&&>(t).map_ref_ref()...);
    f(3, static_cast<T&&>(t).list_ref_unique_ref()...);
    f(4, static_cast<T&&>(t).set_ref_shared_ref()...);
    f(5, static_cast<T&&>(t).list_ref_shared_const_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithSharedConst> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).opt_shared_const_ref()...);
    f(1, static_cast<T&&>(t).shared_const_ref()...);
    f(2, static_cast<T&&>(t).req_shared_const_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::Empty> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
  }
};

template <>
struct ForEachField<::cpp2::StructWithRef> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).def_field_ref()...);
    f(1, static_cast<T&&>(t).opt_field_ref()...);
    f(2, static_cast<T&&>(t).req_field_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithBox> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).a_ref()...);
    f(1, static_cast<T&&>(t).b_ref()...);
    f(2, static_cast<T&&>(t).c_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithNonOptionalBox> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).a_ref()...);
    f(1, static_cast<T&&>(t).b_ref()...);
    f(2, static_cast<T&&>(t).c_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithInternBox> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).field1_ref()...);
    f(1, static_cast<T&&>(t).field2_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithRefTypeUnique> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).def_field_ref()...);
    f(1, static_cast<T&&>(t).opt_field_ref()...);
    f(2, static_cast<T&&>(t).req_field_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithRefTypeShared> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).def_field_ref()...);
    f(1, static_cast<T&&>(t).opt_field_ref()...);
    f(2, static_cast<T&&>(t).req_field_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithRefTypeSharedConst> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).def_field_ref()...);
    f(1, static_cast<T&&>(t).opt_field_ref()...);
    f(2, static_cast<T&&>(t).req_field_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithRefAndAnnotCppNoexceptMoveCtor> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).def_field_ref()...);
  }
};

template <>
struct ForEachField<::cpp2::StructWithString> {
  template <typename F, typename... T>
  void operator()(FOLLY_MAYBE_UNUSED F&& f, FOLLY_MAYBE_UNUSED T&&... t) const {
    f(0, static_cast<T&&>(t).def_unique_string_ref_ref()...);
    f(1, static_cast<T&&>(t).def_shared_string_ref_ref()...);
    f(2, static_cast<T&&>(t).def_shared_string_const_ref_ref()...);
    f(3, static_cast<T&&>(t).unique_string_ref_ref()...);
    f(4, static_cast<T&&>(t).shared_string_ref_ref()...);
  }
};
} // namespace detail
} // namespace thrift
} // namespace apache
