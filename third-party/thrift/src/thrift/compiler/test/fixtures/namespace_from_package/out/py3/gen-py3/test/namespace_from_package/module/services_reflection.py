#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/namespace_from_package/src/module.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#

from thrift.py3.reflection import (
  ArgumentSpec as __ArgumentSpec,
  InterfaceSpec as __InterfaceSpec,
  MethodSpec as __MethodSpec,
  NumberType as __NumberType,
)

import folly.iobuf as _fbthrift_iobuf


import test.namespace_from_package.module.types as _test_namespace_from_package_module_types


def get_reflection__TestService(for_clients: bool):
    spec: __InterfaceSpec = __InterfaceSpec(
        name="TestService",
        methods=None,
        annotations={
        },
    )
    spec.add_method(
        __MethodSpec.__new__(
            __MethodSpec,
            name="init",
            arguments=(
                __ArgumentSpec.__new__(
                    __ArgumentSpec,
                    name="int1",
                    type=int,
                    kind=__NumberType.I64,
                    annotations={
                    },
                ),
            ),
            result=int,
            result_kind=__NumberType.I64,
            exceptions=(
            ),
            annotations={
            },
        )
    )
    return spec
