
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

#include <thrift/compiler/test/fixtures/enums/gen-cpp2/module_types.h>

namespace apache {
namespace thrift {
namespace python {
namespace capi {
template <>
struct Extractor<::test::fixtures::enums::SomeStruct>
    : public BaseExtractor<::test::fixtures::enums::SomeStruct> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::test::fixtures::enums::SomeStruct> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::SomeStruct>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::SomeStruct>> {
  ExtractorResult<::test::fixtures::enums::SomeStruct> operator()(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::SomeStruct>
    : public BaseConstructor<::test::fixtures::enums::SomeStruct> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::test::fixtures::enums::SomeStruct& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::SomeStruct>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::SomeStruct>> {
  PyObject* operator()(const ::test::fixtures::enums::SomeStruct& val);
};

template <>
struct Extractor<::test::fixtures::enums::MyStruct>
    : public BaseExtractor<::test::fixtures::enums::MyStruct> {
  static const bool kUsingMarshal = true;
  ExtractorResult<::test::fixtures::enums::MyStruct> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Extractor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::MyStruct>>
    : public BaseExtractor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::MyStruct>> {
  ExtractorResult<::test::fixtures::enums::MyStruct> operator()(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyStruct>
    : public BaseConstructor<::test::fixtures::enums::MyStruct> {
  static const bool kUsingMarshal = true;
  PyObject* operator()(const ::test::fixtures::enums::MyStruct& val);
};

template <>
struct Constructor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::MyStruct>>
    : public BaseConstructor<::apache::thrift::python::capi::ComposedStruct<
        ::test::fixtures::enums::MyStruct>> {
  PyObject* operator()(const ::test::fixtures::enums::MyStruct& val);
};

template <>
struct Extractor<::test::fixtures::enums::Metasyntactic>
    : public BaseExtractor<::test::fixtures::enums::Metasyntactic> {
  ExtractorResult<::test::fixtures::enums::Metasyntactic> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::Metasyntactic> {
  PyObject* operator()(::test::fixtures::enums::Metasyntactic val);
};

template <>
struct Extractor<::test::fixtures::enums::MyEnum1>
    : public BaseExtractor<::test::fixtures::enums::MyEnum1> {
  ExtractorResult<::test::fixtures::enums::MyEnum1> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyEnum1> {
  PyObject* operator()(::test::fixtures::enums::MyEnum1 val);
};

template <>
struct Extractor<::test::fixtures::enums::MyEnum2>
    : public BaseExtractor<::test::fixtures::enums::MyEnum2> {
  ExtractorResult<::test::fixtures::enums::MyEnum2> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyEnum2> {
  PyObject* operator()(::test::fixtures::enums::MyEnum2 val);
};

template <>
struct Extractor<::test::fixtures::enums::MyEnum3>
    : public BaseExtractor<::test::fixtures::enums::MyEnum3> {
  ExtractorResult<::test::fixtures::enums::MyEnum3> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyEnum3> {
  PyObject* operator()(::test::fixtures::enums::MyEnum3 val);
};

template <>
struct Extractor<::test::fixtures::enums::MyEnum4>
    : public BaseExtractor<::test::fixtures::enums::MyEnum4> {
  ExtractorResult<::test::fixtures::enums::MyEnum4> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyEnum4> {
  PyObject* operator()(::test::fixtures::enums::MyEnum4 val);
};

template <>
struct Extractor<::test::fixtures::enums::MyBitmaskEnum1>
    : public BaseExtractor<::test::fixtures::enums::MyBitmaskEnum1> {
  ExtractorResult<::test::fixtures::enums::MyBitmaskEnum1> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyBitmaskEnum1> {
  PyObject* operator()(::test::fixtures::enums::MyBitmaskEnum1 val);
};

template <>
struct Extractor<::test::fixtures::enums::MyBitmaskEnum2>
    : public BaseExtractor<::test::fixtures::enums::MyBitmaskEnum2> {
  ExtractorResult<::test::fixtures::enums::MyBitmaskEnum2> operator()(PyObject* obj);
  int typeCheck(PyObject* obj);
};

template <>
struct Constructor<::test::fixtures::enums::MyBitmaskEnum2> {
  PyObject* operator()(::test::fixtures::enums::MyBitmaskEnum2 val);
};

} // namespace capi
} // namespace python
} // namespace thrift
} // namespace apache
