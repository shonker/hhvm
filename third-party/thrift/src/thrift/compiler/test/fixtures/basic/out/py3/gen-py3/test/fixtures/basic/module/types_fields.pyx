#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/basic/src/module.thrift
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
cdef class __MyStruct_FieldsSetter(__StructFieldsSetter):

    @staticmethod
    cdef __MyStruct_FieldsSetter _fbthrift_create(_test_fixtures_basic_module_types.cMyStruct* struct_cpp_obj):
        cdef __MyStruct_FieldsSetter __fbthrift_inst = __MyStruct_FieldsSetter.__new__(__MyStruct_FieldsSetter)
        __fbthrift_inst._struct_cpp_obj = struct_cpp_obj
        __fbthrift_inst._setters[__cstring_view(<const char*>"MyIntField")] = __MyStruct_FieldsSetter._set_field_0
        __fbthrift_inst._setters[__cstring_view(<const char*>"MyStringField")] = __MyStruct_FieldsSetter._set_field_1
        __fbthrift_inst._setters[__cstring_view(<const char*>"MyDataField")] = __MyStruct_FieldsSetter._set_field_2
        __fbthrift_inst._setters[__cstring_view(<const char*>"myEnum")] = __MyStruct_FieldsSetter._set_field_3
        __fbthrift_inst._setters[__cstring_view(<const char*>"oneway")] = __MyStruct_FieldsSetter._set_field_4
        __fbthrift_inst._setters[__cstring_view(<const char*>"readonly")] = __MyStruct_FieldsSetter._set_field_5
        __fbthrift_inst._setters[__cstring_view(<const char*>"idempotent")] = __MyStruct_FieldsSetter._set_field_6
        __fbthrift_inst._setters[__cstring_view(<const char*>"floatSet")] = __MyStruct_FieldsSetter._set_field_7
        __fbthrift_inst._setters[__cstring_view(<const char*>"no_hack_codegen_field")] = __MyStruct_FieldsSetter._set_field_8
        return __fbthrift_inst

    cdef void set_field(__MyStruct_FieldsSetter self, const char* name, object value) except *:
        cdef __cstring_view cname = __cstring_view(name)
        cdef cumap[__cstring_view, __MyStruct_FieldsSetterFunc].iterator found = self._setters.find(cname)
        if found == self._setters.end():
            raise TypeError(f"invalid field name {name.decode('utf-8')}")
        deref(found).second(self, value)

    cdef void _set_field_0(self, _fbthrift_value) except *:
        # for field MyIntField
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 0)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'MyIntField is not a { int !r}.')
        _fbthrift_value = <cint64_t> _fbthrift_value
        deref(self._struct_cpp_obj).MyIntField_ref().assign(_fbthrift_value)

    cdef void _set_field_1(self, _fbthrift_value) except *:
        # for field MyStringField
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 1)
            return
        if not isinstance(_fbthrift_value, str):
            raise TypeError(f'MyStringField is not a { str !r}.')
        deref(self._struct_cpp_obj).MyStringField_ref().assign(cmove(bytes_to_string(_fbthrift_value.encode('utf-8'))))

    cdef void _set_field_2(self, _fbthrift_value) except *:
        # for field MyDataField
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 2)
            return
        if not isinstance(_fbthrift_value, _test_fixtures_basic_module_types.MyDataItem):
            raise TypeError(f'MyDataField is not a { _test_fixtures_basic_module_types.MyDataItem !r}.')
        deref(self._struct_cpp_obj).MyDataField_ref().assign(deref((<_test_fixtures_basic_module_types.MyDataItem?> _fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))

    cdef void _set_field_3(self, _fbthrift_value) except *:
        # for field myEnum
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 3)
            return
        if not isinstance(_fbthrift_value, _fbthrift_BadEnum) and not isinstance(_fbthrift_value, _test_fixtures_basic_module_types.MyEnum):
            raise TypeError(f'field myEnum value: {repr(_fbthrift_value)} is not of the enum type { _test_fixtures_basic_module_types.MyEnum }.')
        deref(self._struct_cpp_obj).myEnum_ref().assign(<_test_fixtures_basic_module_types.cMyEnum><int>_fbthrift_value)

    cdef void _set_field_4(self, _fbthrift_value) except *:
        # for field oneway
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 4)
            return
        if not isinstance(_fbthrift_value, bool):
            raise TypeError(f'oneway is not a { bool !r}.')
        deref(self._struct_cpp_obj).oneway_ref().assign(_fbthrift_value)

    cdef void _set_field_5(self, _fbthrift_value) except *:
        # for field readonly
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 5)
            return
        if not isinstance(_fbthrift_value, bool):
            raise TypeError(f'readonly is not a { bool !r}.')
        deref(self._struct_cpp_obj).readonly_ref().assign(_fbthrift_value)

    cdef void _set_field_6(self, _fbthrift_value) except *:
        # for field idempotent
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 6)
            return
        if not isinstance(_fbthrift_value, bool):
            raise TypeError(f'idempotent is not a { bool !r}.')
        deref(self._struct_cpp_obj).idempotent_ref().assign(_fbthrift_value)

    cdef void _set_field_7(self, _fbthrift_value) except *:
        # for field floatSet
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 7)
            return
        deref(self._struct_cpp_obj).floatSet_ref().assign(deref(_test_fixtures_basic_module_types.Set__float(_fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))

    cdef void _set_field_8(self, _fbthrift_value) except *:
        # for field no_hack_codegen_field
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyStruct](deref(self._struct_cpp_obj), 8)
            return
        if not isinstance(_fbthrift_value, str):
            raise TypeError(f'no_hack_codegen_field is not a { str !r}.')
        deref(self._struct_cpp_obj).no_hack_codegen_field_ref().assign(cmove(bytes_to_string(_fbthrift_value.encode('utf-8'))))


@__cython.auto_pickle(False)
cdef class __Containers_FieldsSetter(__StructFieldsSetter):

    @staticmethod
    cdef __Containers_FieldsSetter _fbthrift_create(_test_fixtures_basic_module_types.cContainers* struct_cpp_obj):
        cdef __Containers_FieldsSetter __fbthrift_inst = __Containers_FieldsSetter.__new__(__Containers_FieldsSetter)
        __fbthrift_inst._struct_cpp_obj = struct_cpp_obj
        __fbthrift_inst._setters[__cstring_view(<const char*>"I32List")] = __Containers_FieldsSetter._set_field_0
        __fbthrift_inst._setters[__cstring_view(<const char*>"StringSet")] = __Containers_FieldsSetter._set_field_1
        __fbthrift_inst._setters[__cstring_view(<const char*>"StringToI64Map")] = __Containers_FieldsSetter._set_field_2
        return __fbthrift_inst

    cdef void set_field(__Containers_FieldsSetter self, const char* name, object value) except *:
        cdef __cstring_view cname = __cstring_view(name)
        cdef cumap[__cstring_view, __Containers_FieldsSetterFunc].iterator found = self._setters.find(cname)
        if found == self._setters.end():
            raise TypeError(f"invalid field name {name.decode('utf-8')}")
        deref(found).second(self, value)

    cdef void _set_field_0(self, _fbthrift_value) except *:
        # for field I32List
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cContainers](deref(self._struct_cpp_obj), 0)
            return
        deref(self._struct_cpp_obj).I32List_ref().assign(deref(_test_fixtures_basic_module_types.List__i32(_fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))

    cdef void _set_field_1(self, _fbthrift_value) except *:
        # for field StringSet
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cContainers](deref(self._struct_cpp_obj), 1)
            return
        deref(self._struct_cpp_obj).StringSet_ref().assign(deref(_test_fixtures_basic_module_types.Set__string(_fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))

    cdef void _set_field_2(self, _fbthrift_value) except *:
        # for field StringToI64Map
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cContainers](deref(self._struct_cpp_obj), 2)
            return
        deref(self._struct_cpp_obj).StringToI64Map_ref().assign(deref(_test_fixtures_basic_module_types.Map__string_i64(_fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))


@__cython.auto_pickle(False)
cdef class __MyDataItem_FieldsSetter(__StructFieldsSetter):

    @staticmethod
    cdef __MyDataItem_FieldsSetter _fbthrift_create(_test_fixtures_basic_module_types.cMyDataItem* struct_cpp_obj):
        cdef __MyDataItem_FieldsSetter __fbthrift_inst = __MyDataItem_FieldsSetter.__new__(__MyDataItem_FieldsSetter)
        __fbthrift_inst._struct_cpp_obj = struct_cpp_obj
        return __fbthrift_inst

    cdef void set_field(__MyDataItem_FieldsSetter self, const char* name, object value) except *:
        cdef __cstring_view cname = __cstring_view(name)
        cdef cumap[__cstring_view, __MyDataItem_FieldsSetterFunc].iterator found = self._setters.find(cname)
        if found == self._setters.end():
            raise TypeError(f"invalid field name {name.decode('utf-8')}")
        deref(found).second(self, value)


@__cython.auto_pickle(False)
cdef class __MyException_FieldsSetter(__StructFieldsSetter):

    @staticmethod
    cdef __MyException_FieldsSetter _fbthrift_create(_test_fixtures_basic_module_types.cMyException* struct_cpp_obj):
        cdef __MyException_FieldsSetter __fbthrift_inst = __MyException_FieldsSetter.__new__(__MyException_FieldsSetter)
        __fbthrift_inst._struct_cpp_obj = struct_cpp_obj
        __fbthrift_inst._setters[__cstring_view(<const char*>"MyIntField")] = __MyException_FieldsSetter._set_field_0
        __fbthrift_inst._setters[__cstring_view(<const char*>"MyStringField")] = __MyException_FieldsSetter._set_field_1
        __fbthrift_inst._setters[__cstring_view(<const char*>"myStruct")] = __MyException_FieldsSetter._set_field_2
        __fbthrift_inst._setters[__cstring_view(<const char*>"myUnion")] = __MyException_FieldsSetter._set_field_3
        return __fbthrift_inst

    cdef void set_field(__MyException_FieldsSetter self, const char* name, object value) except *:
        cdef __cstring_view cname = __cstring_view(name)
        cdef cumap[__cstring_view, __MyException_FieldsSetterFunc].iterator found = self._setters.find(cname)
        if found == self._setters.end():
            raise TypeError(f"invalid field name {name.decode('utf-8')}")
        deref(found).second(self, value)

    cdef void _set_field_0(self, _fbthrift_value) except *:
        # for field MyIntField
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyException](deref(self._struct_cpp_obj), 0)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'MyIntField is not a { int !r}.')
        _fbthrift_value = <cint64_t> _fbthrift_value
        deref(self._struct_cpp_obj).MyIntField_ref().assign(_fbthrift_value)

    cdef void _set_field_1(self, _fbthrift_value) except *:
        # for field MyStringField
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyException](deref(self._struct_cpp_obj), 1)
            return
        if not isinstance(_fbthrift_value, str):
            raise TypeError(f'MyStringField is not a { str !r}.')
        deref(self._struct_cpp_obj).MyStringField_ref().assign(cmove(bytes_to_string(_fbthrift_value.encode('utf-8'))))

    cdef void _set_field_2(self, _fbthrift_value) except *:
        # for field myStruct
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyException](deref(self._struct_cpp_obj), 2)
            return
        if not isinstance(_fbthrift_value, _test_fixtures_basic_module_types.MyStruct):
            raise TypeError(f'myStruct is not a { _test_fixtures_basic_module_types.MyStruct !r}.')
        deref(self._struct_cpp_obj).myStruct_ref().assign(deref((<_test_fixtures_basic_module_types.MyStruct?> _fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))

    cdef void _set_field_3(self, _fbthrift_value) except *:
        # for field myUnion
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cMyException](deref(self._struct_cpp_obj), 3)
            return
        if not isinstance(_fbthrift_value, _test_fixtures_basic_module_types.MyUnion):
            raise TypeError(f'myUnion is not a { _test_fixtures_basic_module_types.MyUnion !r}.')
        deref(self._struct_cpp_obj).myUnion_ref().assign(deref((<_test_fixtures_basic_module_types.MyUnion?> _fbthrift_value)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE))


@__cython.auto_pickle(False)
cdef class __ReservedKeyword_FieldsSetter(__StructFieldsSetter):

    @staticmethod
    cdef __ReservedKeyword_FieldsSetter _fbthrift_create(_test_fixtures_basic_module_types.cReservedKeyword* struct_cpp_obj):
        cdef __ReservedKeyword_FieldsSetter __fbthrift_inst = __ReservedKeyword_FieldsSetter.__new__(__ReservedKeyword_FieldsSetter)
        __fbthrift_inst._struct_cpp_obj = struct_cpp_obj
        __fbthrift_inst._setters[__cstring_view(<const char*>"reserved_field")] = __ReservedKeyword_FieldsSetter._set_field_0
        return __fbthrift_inst

    cdef void set_field(__ReservedKeyword_FieldsSetter self, const char* name, object value) except *:
        cdef __cstring_view cname = __cstring_view(name)
        cdef cumap[__cstring_view, __ReservedKeyword_FieldsSetterFunc].iterator found = self._setters.find(cname)
        if found == self._setters.end():
            raise TypeError(f"invalid field name {name.decode('utf-8')}")
        deref(found).second(self, value)

    cdef void _set_field_0(self, _fbthrift_value) except *:
        # for field reserved_field
        if _fbthrift_value is None:
            __reset_field[_test_fixtures_basic_module_types.cReservedKeyword](deref(self._struct_cpp_obj), 0)
            return
        if not isinstance(_fbthrift_value, int):
            raise TypeError(f'reserved_field is not a { int !r}.')
        _fbthrift_value = <cint32_t> _fbthrift_value
        deref(self._struct_cpp_obj).reserved_field_ref().assign(_fbthrift_value)

