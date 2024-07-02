#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/patch/src/module.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#

from libc.stdint cimport (
    int8_t as cint8_t,
    int16_t as cint16_t,
    int32_t as cint32_t,
    int64_t as cint64_t,
    uint16_t as cuint16_t,
    uint32_t as cuint32_t,
)
from libcpp.string cimport string
from libcpp cimport bool as cbool, nullptr, nullptr_t
from cpython cimport bool as pbool
from libcpp.memory cimport shared_ptr, unique_ptr
from libcpp.vector cimport vector
from libcpp.set cimport set as cset
from libcpp.map cimport map as cmap, pair as cpair
from libcpp.unordered_map cimport unordered_map as cumap
cimport folly.iobuf as _fbthrift_iobuf
from thrift.python.exceptions cimport cTException
cimport thrift.py3.exceptions
cimport thrift.py3.types
from thrift.python.std_libcpp cimport string_view as __cstring_view
from thrift.py3.types cimport (
    bstring,
    bytes_to_string,
    field_ref as __field_ref,
    optional_field_ref as __optional_field_ref,
    required_field_ref as __required_field_ref,
    terse_field_ref as __terse_field_ref,
    StructFieldsSetter as __StructFieldsSetter
)
from thrift.python.protocol cimport Protocol as __Protocol
from folly.optional cimport cOptional as __cOptional

cimport apache.thrift.op.patch.types as _apache_thrift_op_patch_types
cimport apache.thrift.type.standard.types as _apache_thrift_type_standard_types

cimport test.fixtures.patch.module.types as _test_fixtures_patch_module_types



ctypedef void (*__MyData_FieldsSetterFunc)(__MyData_FieldsSetter, object) except *

cdef class __MyData_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cMyData* _struct_cpp_obj
    cdef cumap[__cstring_view, __MyData_FieldsSetterFunc] _setters

    @staticmethod
    cdef __MyData_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cMyData* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *
    cdef void _set_field_1(self, _fbthrift_value) except *


ctypedef void (*__MyDataWithCustomDefault_FieldsSetterFunc)(__MyDataWithCustomDefault_FieldsSetter, object) except *

cdef class __MyDataWithCustomDefault_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cMyDataWithCustomDefault* _struct_cpp_obj
    cdef cumap[__cstring_view, __MyDataWithCustomDefault_FieldsSetterFunc] _setters

    @staticmethod
    cdef __MyDataWithCustomDefault_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cMyDataWithCustomDefault* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *
    cdef void _set_field_1(self, _fbthrift_value) except *


ctypedef void (*__MyStruct_FieldsSetterFunc)(__MyStruct_FieldsSetter, object) except *

cdef class __MyStruct_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cMyStruct* _struct_cpp_obj
    cdef cumap[__cstring_view, __MyStruct_FieldsSetterFunc] _setters

    @staticmethod
    cdef __MyStruct_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cMyStruct* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *
    cdef void _set_field_1(self, _fbthrift_value) except *
    cdef void _set_field_2(self, _fbthrift_value) except *
    cdef void _set_field_3(self, _fbthrift_value) except *
    cdef void _set_field_4(self, _fbthrift_value) except *
    cdef void _set_field_5(self, _fbthrift_value) except *
    cdef void _set_field_6(self, _fbthrift_value) except *
    cdef void _set_field_7(self, _fbthrift_value) except *
    cdef void _set_field_8(self, _fbthrift_value) except *
    cdef void _set_field_9(self, _fbthrift_value) except *
    cdef void _set_field_10(self, _fbthrift_value) except *
    cdef void _set_field_11(self, _fbthrift_value) except *
    cdef void _set_field_12(self, _fbthrift_value) except *
    cdef void _set_field_13(self, _fbthrift_value) except *
    cdef void _set_field_14(self, _fbthrift_value) except *
    cdef void _set_field_15(self, _fbthrift_value) except *
    cdef void _set_field_16(self, _fbthrift_value) except *
    cdef void _set_field_17(self, _fbthrift_value) except *
    cdef void _set_field_18(self, _fbthrift_value) except *
    cdef void _set_field_19(self, _fbthrift_value) except *
    cdef void _set_field_20(self, _fbthrift_value) except *
    cdef void _set_field_21(self, _fbthrift_value) except *
    cdef void _set_field_22(self, _fbthrift_value) except *
    cdef void _set_field_23(self, _fbthrift_value) except *
    cdef void _set_field_24(self, _fbthrift_value) except *
    cdef void _set_field_25(self, _fbthrift_value) except *
    cdef void _set_field_26(self, _fbthrift_value) except *
    cdef void _set_field_27(self, _fbthrift_value) except *
    cdef void _set_field_28(self, _fbthrift_value) except *
    cdef void _set_field_29(self, _fbthrift_value) except *
    cdef void _set_field_30(self, _fbthrift_value) except *
    cdef void _set_field_31(self, _fbthrift_value) except *
    cdef void _set_field_32(self, _fbthrift_value) except *


ctypedef void (*__LateDefStruct_FieldsSetterFunc)(__LateDefStruct_FieldsSetter, object) except *

cdef class __LateDefStruct_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cLateDefStruct* _struct_cpp_obj
    cdef cumap[__cstring_view, __LateDefStruct_FieldsSetterFunc] _setters

    @staticmethod
    cdef __LateDefStruct_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cLateDefStruct* struct_cpp_obj)


ctypedef void (*__Recursive_FieldsSetterFunc)(__Recursive_FieldsSetter, object) except *

cdef class __Recursive_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cRecursive* _struct_cpp_obj
    cdef cumap[__cstring_view, __Recursive_FieldsSetterFunc] _setters

    @staticmethod
    cdef __Recursive_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cRecursive* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *


ctypedef void (*__Bar_FieldsSetterFunc)(__Bar_FieldsSetter, object) except *

cdef class __Bar_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cBar* _struct_cpp_obj
    cdef cumap[__cstring_view, __Bar_FieldsSetterFunc] _setters

    @staticmethod
    cdef __Bar_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cBar* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *


ctypedef void (*__Loop_FieldsSetterFunc)(__Loop_FieldsSetter, object) except *

cdef class __Loop_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cLoop* _struct_cpp_obj
    cdef cumap[__cstring_view, __Loop_FieldsSetterFunc] _setters

    @staticmethod
    cdef __Loop_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cLoop* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *


ctypedef void (*__RefFields_FieldsSetterFunc)(__RefFields_FieldsSetter, object) except *

cdef class __RefFields_FieldsSetter(__StructFieldsSetter):
    cdef _test_fixtures_patch_module_types.cRefFields* _struct_cpp_obj
    cdef cumap[__cstring_view, __RefFields_FieldsSetterFunc] _setters

    @staticmethod
    cdef __RefFields_FieldsSetter _fbthrift_create(_test_fixtures_patch_module_types.cRefFields* struct_cpp_obj)
    cdef void _set_field_0(self, _fbthrift_value) except *
    cdef void _set_field_1(self, _fbthrift_value) except *
    cdef void _set_field_2(self, _fbthrift_value) except *
    cdef void _set_field_3(self, _fbthrift_value) except *
    cdef void _set_field_4(self, _fbthrift_value) except *
    cdef void _set_field_5(self, _fbthrift_value) except *
    cdef void _set_field_6(self, _fbthrift_value) except *

