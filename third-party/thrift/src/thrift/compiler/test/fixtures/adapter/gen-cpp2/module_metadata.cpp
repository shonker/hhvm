/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/adapter/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#include <thrift/lib/cpp2/gen/module_metadata_cpp.h>
#include "thrift/compiler/test/fixtures/adapter/gen-cpp2/module_metadata.h"

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


const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::MyAnnotation>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.MyAnnotation", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_MyAnnotation = res.first->second;
  module_MyAnnotation.name() = "module.MyAnnotation";
  module_MyAnnotation.is_union() = false;
  static const auto* const
  module_MyAnnotation_fields = new std::array<EncodedThriftField, 1>{{
    {1, "signature", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_MyAnnotation_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_MyAnnotation.fields()->push_back(std::move(field));
  }
  module_MyAnnotation.structured_annotations()->push_back(*cvStruct("python.Adapter", {{"name", cvString(R"(my.module.Adapter2)")}, {"typeHint", cvString(R"(my.another.module.AdaptedType2[])")}}).cv_struct_ref());
  module_MyAnnotation.structured_annotations()->push_back(*cvStruct("scope.Transitive", {}).cv_struct_ref());
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::Foo>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.Foo", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_Foo = res.first->second;
  module_Foo.name() = "module.Foo";
  module_Foo.is_union() = false;
  static const auto* const
  module_Foo_fields = new std::array<EncodedThriftField, 11>{{
    {1, "intField", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {2, "optionalIntField", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {3, "intFieldWithDefault", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {4, "setField", false, std::make_unique<Typedef>("module.SetWithAdapter", std::make_unique<Set>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE)), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}},
    {5, "optionalSetField", true, std::make_unique<Typedef>("module.SetWithAdapter", std::make_unique<Set>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE)), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}},
    {6, "mapField", false, std::make_unique<Map>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::make_unique<Typedef>("module.ListWithElemAdapter_withAdapter", std::make_unique<Typedef>("module.ListWithElemAdapter_withAdapter", std::make_unique<Typedef>("module.ListWithElemAdapter", std::make_unique<List>(std::make_unique<Typedef>("module.StringWithAdapter", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), })), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{})), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter3)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter3)")}}).cv_struct_ref(), }},
    {7, "optionalMapField", true, std::make_unique<Map>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::make_unique<Typedef>("module.ListWithElemAdapter_withAdapter", std::make_unique<Typedef>("module.ListWithElemAdapter_withAdapter", std::make_unique<Typedef>("module.ListWithElemAdapter", std::make_unique<List>(std::make_unique<Typedef>("module.StringWithAdapter", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), })), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{})), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter3)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter3)")}}).cv_struct_ref(), }},
    {8, "binaryField", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_BINARY_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter3)")}}).cv_struct_ref(), }},
    {9, "longField", false, std::make_unique<Typedef>("module.MyI64", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I64_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("module.MyAnnotation", {{"signature", cvString(R"(MyI64)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}},
    {10, "adaptedLongField", false, std::make_unique<Typedef>("module.MyI64", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I64_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("module.MyAnnotation", {{"signature", cvString(R"(MyI64)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), *cvStruct("python.Adapter", {{"name", cvString(R"(my.Adapter3)")}, {"typeHint", cvString(R"(my.AdaptedType3[])")}}).cv_struct_ref(), }},
    {11, "doubleAdaptedField", false, std::make_unique<Typedef>("module.DoubleTypedefI64", std::make_unique<Typedef>("module.MyI64", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I64_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("module.MyAnnotation", {{"signature", cvString(R"(MyI64)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_Foo_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_Foo.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::Baz>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.Baz", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_Baz = res.first->second;
  module_Baz.name() = "module.Baz";
  module_Baz.is_union() = true;
  static const auto* const
  module_Baz_fields = new std::array<EncodedThriftField, 5>{{
    {1, "intField", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {4, "setField", false, std::make_unique<Typedef>("module.SetWithAdapter", std::make_unique<Set>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE)), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}},
    {6, "mapField", false, std::make_unique<Map>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::make_unique<Typedef>("module.ListWithElemAdapter_withAdapter", std::make_unique<Typedef>("module.ListWithElemAdapter_withAdapter", std::make_unique<Typedef>("module.ListWithElemAdapter", std::make_unique<List>(std::make_unique<Typedef>("module.StringWithAdapter", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), })), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{})), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter3)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter3)")}}).cv_struct_ref(), }},
    {8, "binaryField", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_BINARY_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {9, "longField", false, std::make_unique<Typedef>("module.MyI64", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I64_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("module.MyAnnotation", {{"signature", cvString(R"(MyI64)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_Baz_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_Baz.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::Bar>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.Bar", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_Bar = res.first->second;
  module_Bar.name() = "module.Bar";
  module_Bar.is_union() = false;
  static const auto* const
  module_Bar_fields = new std::array<EncodedThriftField, 7>{{
    {1, "structField", false, std::make_unique<Typedef>("module.Foo", std::make_unique<Struct<::facebook::thrift::test::Foo>>("module.Foo"), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}, {"adaptedType", cvString(R"(::my::Cpp::Type1)")}}).cv_struct_ref(), }},
    {2, "optionalStructField", true, std::make_unique<Typedef>("module.Foo", std::make_unique<Struct<::facebook::thrift::test::Foo>>("module.Foo"), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {3, "structListField", false, std::make_unique<List>(std::make_unique<Typedef>("module.FooWithAdapter", std::make_unique<Typedef>("module.FooWithAdapter", std::make_unique<Struct<::facebook::thrift::test::Foo>>("module.Foo"), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{})), std::vector<ThriftConstStruct>{}},
    {4, "optionalStructListField", true, std::make_unique<List>(std::make_unique<Typedef>("module.FooWithAdapter", std::make_unique<Typedef>("module.FooWithAdapter", std::make_unique<Struct<::facebook::thrift::test::Foo>>("module.Foo"), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{})), std::vector<ThriftConstStruct>{}},
    {5, "unionField", false, std::make_unique<Typedef>("module.Baz", std::make_unique<Union<::facebook::thrift::test::Baz>>("module.Baz"), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {6, "optionalUnionField", true, std::make_unique<Typedef>("module.Baz", std::make_unique<Union<::facebook::thrift::test::Baz>>("module.Baz"), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
    {7, "adaptedStructField", false, std::make_unique<Typedef>("module.DirectlyAdapted", std::make_unique<Struct<::facebook::thrift::test::detail::DirectlyAdapted>>("module.DirectlyAdapted"), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_Bar_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_Bar.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::detail::DirectlyAdapted>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.DirectlyAdapted", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_DirectlyAdapted = res.first->second;
  module_DirectlyAdapted.name() = "module.DirectlyAdapted";
  module_DirectlyAdapted.is_union() = false;
  static const auto* const
  module_DirectlyAdapted_fields = new std::array<EncodedThriftField, 1>{{
    {1, "field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_DirectlyAdapted_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_DirectlyAdapted.fields()->push_back(std::move(field));
  }
  module_DirectlyAdapted.structured_annotations()->push_back(*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter)")}}).cv_struct_ref());
  module_DirectlyAdapted.structured_annotations()->push_back(*cvStruct("python.Adapter", {{"name", cvString(R"(my.module.Adapter)")}, {"typeHint", cvString(R"(my.another.module.AdaptedType)")}}).cv_struct_ref());
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::StructWithFieldAdapter>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.StructWithFieldAdapter", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_StructWithFieldAdapter = res.first->second;
  module_StructWithFieldAdapter.name() = "module.StructWithFieldAdapter";
  module_StructWithFieldAdapter.is_union() = false;
  static const auto* const
  module_StructWithFieldAdapter_fields = new std::array<EncodedThriftField, 5>{{
    {1, "field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("python.Adapter", {{"name", cvString(R"(my.Adapter1)")}, {"typeHint", cvString(R"(my.AdaptedType1)")}}).cv_struct_ref(), }},
    {2, "shared_field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Ref", {{"type", cvInteger(1)}}).cv_struct_ref(), }},
    {3, "opt_shared_field", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Ref", {{"type", cvInteger(1)}}).cv_struct_ref(), }},
    {4, "opt_boxed_field", true, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("thrift.Box", {}).cv_struct_ref(), }},
    {5, "boxed_field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }},
  }};
  for (const auto& f : *module_StructWithFieldAdapter_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_StructWithFieldAdapter.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::TerseAdaptedFields>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.TerseAdaptedFields", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_TerseAdaptedFields = res.first->second;
  module_TerseAdaptedFields.name() = "module.TerseAdaptedFields";
  module_TerseAdaptedFields.is_union() = false;
  static const auto* const
  module_TerseAdaptedFields_fields = new std::array<EncodedThriftField, 3>{{
    {1, "int_field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("thrift.TerseWrite", {}).cv_struct_ref(), }},
    {2, "string_field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("thrift.TerseWrite", {}).cv_struct_ref(), }},
    {3, "set_field", false, std::make_unique<Set>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE)), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), *cvStruct("thrift.TerseWrite", {}).cv_struct_ref(), }},
  }};
  for (const auto& f : *module_TerseAdaptedFields_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_TerseAdaptedFields.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::B>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.B", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_B = res.first->second;
  module_B.name() = "module.B";
  module_B.is_union() = false;
  static const auto* const
  module_B_fields = new std::array<EncodedThriftField, 1>{{
    {1, "a", false, std::make_unique<Typedef>("module.AdaptedA", std::make_unique<Typedef>("module.AdaptedA", std::make_unique<Typedef>("module.A", std::make_unique<Struct<::facebook::thrift::test::A>>("module.A"), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{*cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_B_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_B.fields()->push_back(std::move(field));
  }
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::A>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.A", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_A = res.first->second;
  module_A.name() = "module.A";
  module_A.is_union() = false;
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::Config>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.Config", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_Config = res.first->second;
  module_Config.name() = "module.Config";
  module_Config.is_union() = false;
  static const auto* const
  module_Config_fields = new std::array<EncodedThriftField, 1>{{
    {1, "path", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_Config_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_Config.fields()->push_back(std::move(field));
  }
  module_Config.structured_annotations()->push_back(*cvStruct("cpp.Adapter", {{"name", cvString(R"(MyVarAdapter)")}}).cv_struct_ref());
  module_Config.structured_annotations()->push_back(*cvStruct("python.Adapter", {{"name", cvString(R"(my.ConfigAdapter)")}, {"typeHint", cvString(R"(my.ConfiguredVar[])")}}).cv_struct_ref());
  module_Config.structured_annotations()->push_back(*cvStruct("scope.Transitive", {}).cv_struct_ref());
  module_Config.structured_annotations()->push_back(*cvStruct("thrift.Experimental", {}).cv_struct_ref());
  return res.first->second;
}
const ::apache::thrift::metadata::ThriftStruct&
StructMetadata<::facebook::thrift::test::MyStruct>::gen(ThriftMetadata& metadata) {
  auto res = metadata.structs()->emplace("module.MyStruct", ::apache::thrift::metadata::ThriftStruct{});
  if (!res.second) {
    return res.first->second;
  }
  ::apache::thrift::metadata::ThriftStruct& module_MyStruct = res.first->second;
  module_MyStruct.name() = "module.MyStruct";
  module_MyStruct.is_union() = false;
  static const auto* const
  module_MyStruct_fields = new std::array<EncodedThriftField, 2>{{
    {1, "field", false, std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{}},
    {2, "set_string", false, std::make_unique<Typedef>("module.SetWithAdapter", std::make_unique<Set>(std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE)), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter2)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{}},
  }};
  for (const auto& f : *module_MyStruct_fields) {
    ::apache::thrift::metadata::ThriftField field;
    field.id() = f.id;
    field.name() = f.name;
    field.is_optional() = f.is_optional;
    f.metadata_type_interface->writeAndGenType(*field.type(), metadata);
    field.structured_annotations() = f.structured_annotations;
    module_MyStruct.fields()->push_back(std::move(field));
  }
  return res.first->second;
}

void ServiceMetadata<::apache::thrift::ServiceHandler<::facebook::thrift::test::Service>>::gen_func(FOLLY_MAYBE_UNUSED ThriftMetadata& metadata, ThriftService& service) {
  ::apache::thrift::metadata::ThriftFunction func;
  func.name() = "func";
  auto func_ret_type = std::make_unique<Typedef>("module.MyI32", std::make_unique<Typedef>("module.MyI32", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_I32_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{});
  func_ret_type->writeAndGenType(*func.return_type(), metadata);
  ::apache::thrift::metadata::ThriftField module_Service_func_arg1_1;
  module_Service_func_arg1_1.id() = 1;
  module_Service_func_arg1_1.name() = "arg1";
  module_Service_func_arg1_1.is_optional() = false;
  auto module_Service_func_arg1_1_type = std::make_unique<Typedef>("module.StringWithAdapter", std::make_unique<Typedef>("module.StringWithAdapter", std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE), std::vector<ThriftConstStruct>{*cvStruct("hack.Adapter", {{"name", cvString(R"(\Adapter1)")}}).cv_struct_ref(), *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter1)")}}).cv_struct_ref(), }), std::vector<ThriftConstStruct>{});
  module_Service_func_arg1_1_type->writeAndGenType(*module_Service_func_arg1_1.type(), metadata);
  func.arguments()->push_back(std::move(module_Service_func_arg1_1));
  ::apache::thrift::metadata::ThriftField module_Service_func_arg2_2;
  module_Service_func_arg2_2.id() = 2;
  module_Service_func_arg2_2.name() = "arg2";
  module_Service_func_arg2_2.is_optional() = false;
  module_Service_func_arg2_2.structured_annotations() = {
      *cvStruct("cpp.Adapter", {{"name", cvString(R"(::my::Adapter2)")}}).cv_struct_ref(),
  };
  auto module_Service_func_arg2_2_type = std::make_unique<Primitive>(ThriftPrimitiveType::THRIFT_STRING_TYPE);
  module_Service_func_arg2_2_type->writeAndGenType(*module_Service_func_arg2_2.type(), metadata);
  func.arguments()->push_back(std::move(module_Service_func_arg2_2));
  ::apache::thrift::metadata::ThriftField module_Service_func_arg3_3;
  module_Service_func_arg3_3.id() = 3;
  module_Service_func_arg3_3.name() = "arg3";
  module_Service_func_arg3_3.is_optional() = false;
  auto module_Service_func_arg3_3_type = std::make_unique<Struct<::facebook::thrift::test::Foo>>("module.Foo");
  module_Service_func_arg3_3_type->writeAndGenType(*module_Service_func_arg3_3.type(), metadata);
  func.arguments()->push_back(std::move(module_Service_func_arg3_3));
  func.is_oneway() = false;
  service.functions()->push_back(std::move(func));
}

void ServiceMetadata<::apache::thrift::ServiceHandler<::facebook::thrift::test::Service>>::gen(::apache::thrift::metadata::ThriftServiceMetadataResponse& response) {
  const ::apache::thrift::metadata::ThriftServiceContextRef* self = genRecurse(*response.metadata(), *response.services());
  DCHECK(self != nullptr);
  // TODO(praihan): Remove ThriftServiceContext from response. But in the meantime, we need to fill the field with the result of looking up in ThriftMetadata.
  ::apache::thrift::metadata::ThriftServiceContext context;
  context.module() = *self->module();
  context.service_info() = response.metadata()->services()->at(*self->service_name());
  response.context() = std::move(context);
}

const ThriftServiceContextRef* ServiceMetadata<::apache::thrift::ServiceHandler<::facebook::thrift::test::Service>>::genRecurse(FOLLY_MAYBE_UNUSED ThriftMetadata& metadata, std::vector<ThriftServiceContextRef>& services) {
  ::apache::thrift::metadata::ThriftService module_Service;
  module_Service.name() = "module.Service";
  static const ThriftFunctionGenerator functions[] = {
    ServiceMetadata<::apache::thrift::ServiceHandler<::facebook::thrift::test::Service>>::gen_func,
  };
  for (auto& function_gen : functions) {
    function_gen(metadata, module_Service);
  }
  // We need to keep the index around because a reference or iterator could be invalidated.
  auto selfIndex = services.size();
  services.emplace_back();
  ThriftServiceContextRef& context = services[selfIndex];
  metadata.services()->emplace("module.Service", std::move(module_Service));
  context.service_name() = "module.Service";
  ::apache::thrift::metadata::ThriftModuleContext module;
  module.name() = "module";
  context.module() = std::move(module);
  return &context;
}
} // namespace md
} // namespace detail
} // namespace thrift
} // namespace apache
