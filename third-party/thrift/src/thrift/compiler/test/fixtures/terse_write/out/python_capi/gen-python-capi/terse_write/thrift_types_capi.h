
/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT
 *  @generated
 *
 */

#pragma once

#include <thrift/lib/python/capi/constructor.h>
#include <thrift/lib/python/capi/extractor.h>

#include <thrift/compiler/test/fixtures/terse_write/gen-cpp2/terse_write_types.h>

namespace apache {
namespace thrift {
namespace python {
namespace capi {
template <>
struct Extractor<::facebook::thrift::test::terse_write::MyStruct>
    : public BaseExtractor<::facebook::thrift::test::terse_write::MyStruct> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::MyStruct> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStruct>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStruct>> {
  ExtractorResult<::facebook::thrift::test::terse_write::MyStruct> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::MyStruct>
    : public BaseConstructor<::facebook::thrift::test::terse_write::MyStruct> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::MyStruct& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStruct>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStruct>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::MyStruct& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::MyUnion>
    : public BaseExtractor<::facebook::thrift::test::terse_write::MyUnion> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::MyUnion> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyUnion>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyUnion>> {
  ExtractorResult<::facebook::thrift::test::terse_write::MyUnion> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::MyUnion>
    : public BaseConstructor<::facebook::thrift::test::terse_write::MyUnion> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::MyUnion& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyUnion>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyUnion>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::MyUnion& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::MyStructWithCustomDefault>
    : public BaseExtractor<::facebook::thrift::test::terse_write::MyStructWithCustomDefault> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::MyStructWithCustomDefault> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStructWithCustomDefault>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStructWithCustomDefault>> {
  ExtractorResult<::facebook::thrift::test::terse_write::MyStructWithCustomDefault> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::MyStructWithCustomDefault>
    : public BaseConstructor<::facebook::thrift::test::terse_write::MyStructWithCustomDefault> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::MyStructWithCustomDefault& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStructWithCustomDefault>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::MyStructWithCustomDefault>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::MyStructWithCustomDefault& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::StructLevelTerseStruct>
    : public BaseExtractor<::facebook::thrift::test::terse_write::StructLevelTerseStruct> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::StructLevelTerseStruct> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::StructLevelTerseStruct>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::StructLevelTerseStruct>> {
  ExtractorResult<::facebook::thrift::test::terse_write::StructLevelTerseStruct> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::StructLevelTerseStruct>
    : public BaseConstructor<::facebook::thrift::test::terse_write::StructLevelTerseStruct> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::StructLevelTerseStruct& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::StructLevelTerseStruct>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::StructLevelTerseStruct>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::StructLevelTerseStruct& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::FieldLevelTerseStruct>
    : public BaseExtractor<::facebook::thrift::test::terse_write::FieldLevelTerseStruct> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::FieldLevelTerseStruct> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::FieldLevelTerseStruct>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::FieldLevelTerseStruct>> {
  ExtractorResult<::facebook::thrift::test::terse_write::FieldLevelTerseStruct> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::FieldLevelTerseStruct>
    : public BaseConstructor<::facebook::thrift::test::terse_write::FieldLevelTerseStruct> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::FieldLevelTerseStruct& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::FieldLevelTerseStruct>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::FieldLevelTerseStruct>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::FieldLevelTerseStruct& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::TerseStructWithCustomDefault>
    : public BaseExtractor<::facebook::thrift::test::terse_write::TerseStructWithCustomDefault> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::TerseStructWithCustomDefault> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseStructWithCustomDefault>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseStructWithCustomDefault>> {
  ExtractorResult<::facebook::thrift::test::terse_write::TerseStructWithCustomDefault> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::TerseStructWithCustomDefault>
    : public BaseConstructor<::facebook::thrift::test::terse_write::TerseStructWithCustomDefault> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::TerseStructWithCustomDefault& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseStructWithCustomDefault>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseStructWithCustomDefault>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::TerseStructWithCustomDefault& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::AdaptedFields>
    : public BaseExtractor<::facebook::thrift::test::terse_write::AdaptedFields> {
  static const bool kUsingMarshal = false;
  ExtractorResult<::facebook::thrift::test::terse_write::AdaptedFields> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::AdaptedFields>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::AdaptedFields>> {
  ExtractorResult<::facebook::thrift::test::terse_write::AdaptedFields> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::AdaptedFields>
    : public BaseConstructor<::facebook::thrift::test::terse_write::AdaptedFields> {
  static const bool kUsingMarshal = false;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::AdaptedFields& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::AdaptedFields>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::AdaptedFields>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::AdaptedFields& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::WrappedFields>
    : public BaseExtractor<::facebook::thrift::test::terse_write::WrappedFields> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::WrappedFields> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::WrappedFields>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::WrappedFields>> {
  ExtractorResult<::facebook::thrift::test::terse_write::WrappedFields> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::WrappedFields>
    : public BaseConstructor<::facebook::thrift::test::terse_write::WrappedFields> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::WrappedFields& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::WrappedFields>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::WrappedFields>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::WrappedFields& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::TerseException>
    : public BaseExtractor<::facebook::thrift::test::terse_write::TerseException> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::facebook::thrift::test::terse_write::TerseException> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseException>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseException>> {
  ExtractorResult<::facebook::thrift::test::terse_write::TerseException> operator()(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::TerseException>
    : public BaseConstructor<::facebook::thrift::test::terse_write::TerseException> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::facebook::thrift::test::terse_write::TerseException& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseException>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::facebook::thrift::test::terse_write::TerseException>> {
  PyObject* operator()(const ::facebook::thrift::test::terse_write::TerseException& val);
};

template <>
struct Extractor<::facebook::thrift::test::terse_write::MyEnum>
    : public BaseExtractor<::facebook::thrift::test::terse_write::MyEnum> {
  ExtractorResult<::facebook::thrift::test::terse_write::MyEnum> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::facebook::thrift::test::terse_write::MyEnum> {
  PyObject* operator()(::facebook::thrift::test::terse_write::MyEnum val);
};

} // namespace capi
} // namespace python
} // namespace thrift
} // namespace apache
