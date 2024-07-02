/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/constants/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#include <thrift/lib/cpp2/gen/module_metadata_cpp.h>
#include "thrift/compiler/test/fixtures/constants/gen-cpp2/module_metadata.h"

// some of these functions can be so large that the compiler gives up optimizing
// them - and issues a warning which may be treated as an error!
//
// these functions are so rarely called that it is probably okay for them not to
// be optimized in practice
FOLLY_CLANG_DISABLE_WARNING("-Wignored-optimization-argument")

namespace apache {
namespace thrift {
namespace detail {
namespace md {
using ThriftMetadata = ::apache::thrift::metadata::ThriftMetadata;
using ThriftPrimitiveType = ::apache::thrift::metadata::ThriftPrimitiveType;
using ThriftType = ::apache::thrift::metadata::ThriftType;
using ThriftService = ::apache::thrift::metadata::ThriftService;
using ThriftServiceContext = ::apache::thrift::metadata::ThriftServiceContext;
using ThriftFunctionGenerator = void (*)(ThriftMetadata&, ThriftService&);

void EnumMetadata<::cpp2::EmptyEnum>::gen(ThriftMetadata& metadata) {
  auto res = metadata.enums()->emplace("module.EmptyEnum", ::apache::thrift::metadata::ThriftEnum{});
  if (!res.second) {
    return;
  }
  ::apache::thrift::metadata::ThriftEnum& enum_metadata = res.first->second;
  enum_metadata.name() = "module.EmptyEnum";
  using EnumTraits = TEnumTraits<::cpp2::EmptyEnum>;
  for (std::size_t i = 0; i != EnumTraits::size; ++i) {
    enum_metadata.elements()->emplace(static_cast<int32_t>(EnumTraits::values[i]), EnumTraits::names[i]);
  }
}
void EnumMetadata<::cpp2::City>::gen(ThriftMetadata& metadata) {
  auto res = metadata.enums()->emplace("module.City", ::apache::thrift::metadata::ThriftEnum{});
  if (!res.second) {
    return;
  }
  ::apache::thrift::metadata::ThriftEnum& enum_metadata = res.first->second;
  enum_metadata.name() = "module.City";
  using EnumTraits = TEnumTraits<::cpp2::City>;
  for (std::size_t i = 0; i != EnumTraits::size; ++i) {
    enum_metadata.elements()->emplace(static_cast<int32_t>(EnumTraits::values[i]), EnumTraits::names[i]);
  }
}
void EnumMetadata<::cpp2::Company>::gen(ThriftMetadata& metadata) {
  auto res = metadata.enums()->emplace("module.Company", ::apache::thrift::metadata::ThriftEnum{});
  if (!res.second) {
    return;
  }
  ::apache::thrift::metadata::ThriftEnum& enum_metadata = res.first->second;
  enum_metadata.name() = "module.Company";
  using EnumTraits = TEnumTraits<::cpp2::Company>;
  for (std::size_t i = 0; i != EnumTraits::size; ++i) {
    enum_metadata.elements()->emplace(static_cast<int32_t>(EnumTraits::values[i]), EnumTraits::names[i]);
  }
}

const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::Internship>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.Internship", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_Internship = res.first->second;
  module_Internship.name() = "module.Internship";
  module_Internship.is_union() = false;
  static const auto* const
  module_Internship_fields = new std::array<EncodedThriftField, 5>{ {
    { 1, "weeks", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "title", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{ }},    { 3, "employer", true, std::make_unique<Enum<::cpp2::Company>>("module.Company"), std::vector<ThriftConstStruct>{ }},    { 4, "compensation", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_DOUBLE_TYPE), std::vector<ThriftConstStruct>{ }},    { 5, "school", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_Internship_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_Internship.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::Range>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.Range", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_Range = res.first->second;
  module_Range.name() = "module.Range";
  module_Range.is_union() = false;
  static const auto* const
  module_Range_fields = new std::array<EncodedThriftField, 2>{ {
    { 1, "min", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "max", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_Range_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_Range.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::struct1>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.struct1", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_struct1 = res.first->second;
  module_struct1.name() = "module.struct1";
  module_struct1.is_union() = false;
  static const auto* const
  module_struct1_fields = new std::array<EncodedThriftField, 2>{ {
    { 1, "a", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "b", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_struct1_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_struct1.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::struct2>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.struct2", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_struct2 = res.first->second;
  module_struct2.name() = "module.struct2";
  module_struct2.is_union() = false;
  static const auto* const
  module_struct2_fields = new std::array<EncodedThriftField, 4>{ {
    { 1, "a", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "b", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{ }},    { 3, "c", false, std::make_unique<Struct<::cpp2::struct1>>("module.struct1"), std::vector<ThriftConstStruct>{ }},    { 4, "d", false, std::make_unique<List>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE)), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_struct2_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_struct2.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::struct3>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.struct3", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_struct3 = res.first->second;
  module_struct3.name() = "module.struct3";
  module_struct3.is_union() = false;
  static const auto* const
  module_struct3_fields = new std::array<EncodedThriftField, 3>{ {
    { 1, "a", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "b", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 3, "c", false, std::make_unique<Struct<::cpp2::struct2>>("module.struct2"), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_struct3_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_struct3.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::struct4>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.struct4", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_struct4 = res.first->second;
  module_struct4.name() = "module.struct4";
  module_struct4.is_union() = false;
  static const auto* const
  module_struct4_fields = new std::array<EncodedThriftField, 3>{ {
    { 1, "a", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "b", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_DOUBLE_TYPE), std::vector<ThriftConstStruct>{ }},    { 3, "c", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_BYTE_TYPE), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_struct4_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_struct4.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::union1>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.union1", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_union1 = res.first->second;
  module_union1.name() = "module.union1";
  module_union1.is_union() = true;
  static const auto* const
  module_union1_fields = new std::array<EncodedThriftField, 2>{ {
    { 1, "i", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "d", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_DOUBLE_TYPE), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_union1_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_union1.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::cpp2::union2>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.union2", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_union2 = res.first->second;
  module_union2.name() = "module.union2";
  module_union2.is_union() = true;
  static const auto* const
  module_union2_fields = new std::array<EncodedThriftField, 4>{ {
    { 1, "i", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{ }},    { 2, "d", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_DOUBLE_TYPE), std::vector<ThriftConstStruct>{ }},    { 3, "s", false, std::make_unique<Struct<::cpp2::struct1>>("module.struct1"), std::vector<ThriftConstStruct>{ }},    { 4, "u", false, std::make_unique<Union<::cpp2::union1>>("module.union1"), std::vector<ThriftConstStruct>{ }},  }};
  for (const auto& f : *module_union2_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_union2.fields()->push_back(std::move(field));
  }
  return res.first->second;
}

} // namespace md
} // namespace detail
} // namespace thrift
} // namespace apache
