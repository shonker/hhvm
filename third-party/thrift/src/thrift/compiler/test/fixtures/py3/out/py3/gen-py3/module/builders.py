#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/py3/src/module.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#
import typing as _typing

import folly.iobuf as _fbthrift_iobuf
import thrift.py3.builder


import module.types as _module_types


_fbthrift_struct_type__SimpleException = _module_types.SimpleException
class SimpleException_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__SimpleException

    def __init__(self):
        self.err_code: _typing.Optional[int] = None

    def __iter__(self):
        yield "err_code", self.err_code

_fbthrift_struct_type__OptionalRefStruct = _module_types.OptionalRefStruct
class OptionalRefStruct_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__OptionalRefStruct

    def __init__(self):
        self.optional_blob: _typing.Optional[_fbthrift_iobuf.IOBuf] = None

    def __iter__(self):
        yield "optional_blob", self.optional_blob

_fbthrift_struct_type__SimpleStruct = _module_types.SimpleStruct
class SimpleStruct_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__SimpleStruct

    def __init__(self):
        self.is_on: _typing.Optional[bool] = None
        self.tiny_int: _typing.Optional[int] = None
        self.small_int: _typing.Optional[int] = None
        self.nice_sized_int: _typing.Optional[int] = None
        self.big_int: _typing.Optional[int] = None
        self.real: _typing.Optional[float] = None
        self.smaller_real: _typing.Optional[float] = None

    def __iter__(self):
        yield "is_on", self.is_on
        yield "tiny_int", self.tiny_int
        yield "small_int", self.small_int
        yield "nice_sized_int", self.nice_sized_int
        yield "big_int", self.big_int
        yield "real", self.real
        yield "smaller_real", self.smaller_real

_fbthrift_struct_type__HiddenTypeFieldsStruct = _module_types.HiddenTypeFieldsStruct
class HiddenTypeFieldsStruct_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__HiddenTypeFieldsStruct

    def __init__(self):
        pass

    def __iter__(self):
        pass

_fbthrift_struct_type__ComplexStruct = _module_types.ComplexStruct
class ComplexStruct_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__ComplexStruct

    def __init__(self):
        self.structOne: _typing.Any = None
        self.structTwo: _typing.Any = None
        self.an_integer: _typing.Optional[int] = None
        self.name: _typing.Optional[str] = None
        self.an_enum: _typing.Optional[_module_types.AnEnum] = None
        self.some_bytes: _typing.Optional[bytes] = None
        self.sender: _typing.Optional[str] = None
        self.cdef_: _typing.Optional[str] = None
        self.bytes_with_cpp_type: _typing.Optional[bytes] = None

    def __iter__(self):
        yield "structOne", self.structOne
        yield "structTwo", self.structTwo
        yield "an_integer", self.an_integer
        yield "name", self.name
        yield "an_enum", self.an_enum
        yield "some_bytes", self.some_bytes
        yield "sender", self.sender
        yield "cdef_", self.cdef_
        yield "bytes_with_cpp_type", self.bytes_with_cpp_type

_fbthrift_struct_type__BinaryUnion = _module_types.BinaryUnion
class BinaryUnion_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__BinaryUnion

    def __init__(self):
        self.iobuf_val: _typing.Optional[_fbthrift_iobuf.IOBuf] = None

    def __iter__(self):
        yield "iobuf_val", self.iobuf_val

_fbthrift_struct_type__BinaryUnionStruct = _module_types.BinaryUnionStruct
class BinaryUnionStruct_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__BinaryUnionStruct

    def __init__(self):
        self.u: _typing.Any = None

    def __iter__(self):
        yield "u", self.u

