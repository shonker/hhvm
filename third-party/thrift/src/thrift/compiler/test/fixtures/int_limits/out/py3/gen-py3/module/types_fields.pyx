#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/int_limits/src/module.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#
cimport cython as __cython
from cython.operator cimport dereference as deref
from libcpp.utility cimport move as cmove
from thrift.py3.types cimport (
    assign_unique_ptr,
    assign_shared_ptr,
    assign_shared_const_ptr,
    bytes_to_string,
    make_unique,
)
cimport thrift.py3.types
from thrift.py3.types cimport (
    reset_field as __reset_field,
    StructFieldsSetter as __StructFieldsSetter
)

from thrift.py3.types cimport const_pointer_cast, BadEnum as _fbthrift_BadEnum


@__cython.auto_pickle(False)
cdef class __Limits_FieldsSetter(__StructFieldsSetter):

    @staticmethod
    cdef __Limits_FieldsSetter _fbthrift_create(_module_types.cLimits* struct_cpp_obj):
        cdef __Limits_FieldsSetter __fbthrift_inst = __Limits_FieldsSetter.__new__(__Limits_FieldsSetter)
        __fbthrift_inst._struct_cpp_obj = struct_cpp_obj
        __fbthrift_inst._setters[__cstring_view(<const char*>"max_i64_field")] = __Limits_FieldsSetter._set_field_0
        __fbthrift_inst._setters[__cstring_view(<const char*>"min_i64_field")] = __Limits_FieldsSetter._set_field_1
        __fbthrift_inst._setters[__cstring_view(<const char*>"max_i32_field")] = __Limits_FieldsSetter._set_field_2
        __fbthrift_inst._setters[__cstring_view(<const char*>"min_i32_field")] = __Limits_FieldsSetter._set_field_3
        __fbthrift_inst._setters[__cstring_view(<const char*>"max_i16_field")] = __Limits_FieldsSetter._set_field_4
        __fbthrift_inst._setters[__cstring_view(<const char*>"min_i16_field")] = __Limits_FieldsSetter._set_field_5
        __fbthrift_inst._setters[__cstring_view(<const char*>"max_byte_field")] = __Limits_FieldsSetter._set_field_6
        __fbthrift_inst._setters[__cstring_view(<const char*>"min_byte_field")] = __Limits_FieldsSetter._set_field_7
        return __fbthrift_inst

    cdef void set_field(__Limits_FieldsSetter self, const char* name, object value) except *:
        cdef __cstring_view cname = __cstring_view(name)
        cdef cumap[__cstring_view, __Limits_FieldsSetterFunc].iterator found = self._setters.find(cname)
        if found == self._setters.end():
            raise TypeError(f"invalid field name {name.decode('utf-8')}")
        deref(found).second(self, value)

    cdef void _set_field_0(self, _fbthrift_value) except *:
        # for field max_i64_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 0)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'max_i64_field is not a { int !r}.')
        _fbthrift_value = <cint64_t> _fbthrift_value
        deref(self._struct_cpp_obj).max_i64_field_ref().assign(_fbthrift_value)

    cdef void _set_field_1(self, _fbthrift_value) except *:
        # for field min_i64_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 1)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'min_i64_field is not a { int !r}.')
        _fbthrift_value = <cint64_t> _fbthrift_value
        deref(self._struct_cpp_obj).min_i64_field_ref().assign(_fbthrift_value)

    cdef void _set_field_2(self, _fbthrift_value) except *:
        # for field max_i32_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 2)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'max_i32_field is not a { int !r}.')
        _fbthrift_value = <cint32_t> _fbthrift_value
        deref(self._struct_cpp_obj).max_i32_field_ref().assign(_fbthrift_value)

    cdef void _set_field_3(self, _fbthrift_value) except *:
        # for field min_i32_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 3)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'min_i32_field is not a { int !r}.')
        _fbthrift_value = <cint32_t> _fbthrift_value
        deref(self._struct_cpp_obj).min_i32_field_ref().assign(_fbthrift_value)

    cdef void _set_field_4(self, _fbthrift_value) except *:
        # for field max_i16_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 4)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'max_i16_field is not a { int !r}.')
        _fbthrift_value = <cint16_t> _fbthrift_value
        deref(self._struct_cpp_obj).max_i16_field_ref().assign(_fbthrift_value)

    cdef void _set_field_5(self, _fbthrift_value) except *:
        # for field min_i16_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 5)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'min_i16_field is not a { int !r}.')
        _fbthrift_value = <cint16_t> _fbthrift_value
        deref(self._struct_cpp_obj).min_i16_field_ref().assign(_fbthrift_value)

    cdef void _set_field_6(self, _fbthrift_value) except *:
        # for field max_byte_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 6)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'max_byte_field is not a { int !r}.')
        _fbthrift_value = <cint8_t> _fbthrift_value
        deref(self._struct_cpp_obj).max_byte_field_ref().assign(_fbthrift_value)

    cdef void _set_field_7(self, _fbthrift_value) except *:
        # for field min_byte_field
        if _fbthrift_value is None:
            __reset_field[_module_types.cLimits](deref(self._struct_cpp_obj), 7)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'min_byte_field is not a { int !r}.')
        _fbthrift_value = <cint8_t> _fbthrift_value
        deref(self._struct_cpp_obj).min_byte_field_ref().assign(_fbthrift_value)

