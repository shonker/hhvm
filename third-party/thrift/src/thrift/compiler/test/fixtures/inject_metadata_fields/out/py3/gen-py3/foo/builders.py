#
# Autogenerated by Thrift for foo.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#
import typing as _typing

import folly.iobuf as _fbthrift_iobuf
import thrift.py3.builder


import foo.types as _foo_types


_fbthrift_struct_type__Fields = _foo_types.Fields
class Fields_Builder(thrift.py3.builder.StructBuilder):
    _struct_type = _fbthrift_struct_type__Fields

    def __init__(self):
        self.injected_field: _typing.Optional[str] = None
        self.injected_structured_annotation_field: _typing.Optional[str] = None
        self.injected_unstructured_annotation_field: _typing.Optional[str] = None

    def __iter__(self):
        yield "injected_field", self.injected_field
        yield "injected_structured_annotation_field", self.injected_structured_annotation_field
        yield "injected_unstructured_annotation_field", self.injected_unstructured_annotation_field

