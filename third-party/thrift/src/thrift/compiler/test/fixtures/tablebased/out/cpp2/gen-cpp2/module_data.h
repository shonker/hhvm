/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/tablebased/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include <thrift/lib/cpp2/gen/module_data_h.h>

#include "thrift/compiler/test/fixtures/tablebased/gen-cpp2/module_types.h"

namespace apache { namespace thrift {

template <> struct TEnumDataStorage<::test::fixtures::tablebased::ExampleEnum> {
  using type = ::test::fixtures::tablebased::ExampleEnum;
  static constexpr const std::size_t size = 2;
  static constexpr std::array<type, size> values = { {
      type::ZERO,
      type::NONZERO,
  }};
  static constexpr std::array<std::string_view, size> names = { {
      "ZERO"sv,
      "NONZERO"sv,
  }};
};

template <> struct TEnumDataStorage<::test::fixtures::tablebased::ExampleUnion::Type> {
  using type = ::test::fixtures::tablebased::ExampleUnion::Type;
  static constexpr const std::size_t size = 2;
  static constexpr std::array<type, size> values = { {
      type::fieldA,
      type::fieldB,
  }};
  static constexpr std::array<std::string_view, size> names = { {
      "fieldA"sv,
      "fieldB"sv,
  }};
};

template <> struct TStructDataStorage<::test::fixtures::tablebased::TrivialTypesStruct> {
  static constexpr const std::size_t fields_size = 5;
  static const std::string_view name;
  static const std::array<std::string_view, fields_size> fields_names;
  static const std::array<int16_t, fields_size> fields_ids;
  static const std::array<protocol::TType, fields_size> fields_types;

 private:
  // The following fields describe internal storage metadata, and are private to
  // prevent user logic from accessing them, but they can be inspected by
  // debuggers.
  static const std::array<std::string_view, fields_size> storage_names;
  // -1 if the field has no isset.
  static const std::array<int, fields_size> isset_indexes;
};

template <> struct TStructDataStorage<::test::fixtures::tablebased::ContainerStruct> {
  static constexpr const std::size_t fields_size = 8;
  static const std::string_view name;
  static const std::array<std::string_view, fields_size> fields_names;
  static const std::array<int16_t, fields_size> fields_ids;
  static const std::array<protocol::TType, fields_size> fields_types;

 private:
  // The following fields describe internal storage metadata, and are private to
  // prevent user logic from accessing them, but they can be inspected by
  // debuggers.
  static const std::array<std::string_view, fields_size> storage_names;
  // -1 if the field has no isset.
  static const std::array<int, fields_size> isset_indexes;
};

template <> struct TStructDataStorage<::test::fixtures::tablebased::ExampleUnion> {
  static constexpr const std::size_t fields_size = 2;
  static const std::string_view name;
  static const std::array<std::string_view, fields_size> fields_names;
  static const std::array<int16_t, fields_size> fields_ids;
  static const std::array<protocol::TType, fields_size> fields_types;

 private:
  // The following fields describe internal storage metadata, and are private to
  // prevent user logic from accessing them, but they can be inspected by
  // debuggers.
  static const std::array<std::string_view, fields_size> storage_names;
  // -1 if the field has no isset.
  static const std::array<int, fields_size> isset_indexes;
};

}} // apache::thrift
