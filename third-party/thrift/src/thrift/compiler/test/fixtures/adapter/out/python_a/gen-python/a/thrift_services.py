#
# Autogenerated by Thrift
#
# DO NOT EDIT
#  @generated
#

from __future__ import annotations

from abc import ABCMeta
import typing as _typing

import folly.iobuf as _fbthrift_iobuf

import apache.thrift.metadata.thrift_types as _fbthrift_metadata
from thrift.python.serializer import serialize_iobuf, deserialize, Protocol
from thrift.python.server import ServiceInterface, RpcKind, PythonUserException

import a.thrift_types
import a.thrift_metadata
import b.thrift_types
import c.thrift_types
import three
import two
import one
import typeshed_three
import typeshed_two
import typeshed_one

class MyServiceInterface(
    ServiceInterface,
    metaclass=ABCMeta
):

    @staticmethod
    def service_name() -> bytes:
        return b"MyService"

    def getFunctionTable(self) -> _typing.Mapping[bytes, _typing.Callable[..., object]]:
        functionTable = {
            b"adapted_return": (RpcKind.SINGLE_REQUEST_SINGLE_RESPONSE, self._fbthrift__handler_adapted_return),
            b"adapted_param": (RpcKind.SINGLE_REQUEST_SINGLE_RESPONSE, self._fbthrift__handler_adapted_param),
        }
        return {**super().getFunctionTable(), **functionTable}

    @staticmethod
    def __get_thrift_name__() -> str:
        return "a.MyService"

    @staticmethod
    def __get_metadata__() -> _fbthrift_metadata.ThriftMetadata:
        return a.thrift_metadata.gen_metadata_service_MyService()

    @staticmethod
    def __get_metadata_service_response__() -> _fbthrift_metadata.ThriftServiceMetadataResponse:
        return a.thrift_metadata._fbthrift_metadata_service_response_MyService()



    async def adapted_return(
            self
        ) -> typeshed_one.AdapterOneType[b.thrift_types.B]:
        raise NotImplementedError("async def adapted_return is not implemented")

    async def _fbthrift__handler_adapted_return(self, args: _fbthrift_iobuf.IOBuf, protocol: Protocol) -> _fbthrift_iobuf.IOBuf:
        args_struct = deserialize(a.thrift_types._fbthrift_MyService_adapted_return_args, args, protocol)
        value = await self.adapted_return()
        return_struct = a.thrift_types._fbthrift_MyService_adapted_return_result(success=value)
        

        return serialize_iobuf(return_struct, protocol)


    async def adapted_param(
            self,
            param: typeshed_two.AdapterTwoType[c.thrift_types.C2]
        ) -> None:
        raise NotImplementedError("async def adapted_param is not implemented")

    async def _fbthrift__handler_adapted_param(self, args: _fbthrift_iobuf.IOBuf, protocol: Protocol) -> _fbthrift_iobuf.IOBuf:
        args_struct = deserialize(a.thrift_types._fbthrift_MyService_adapted_param_args, args, protocol)
        value = await self.adapted_param(args_struct.param,)
        return_struct = a.thrift_types._fbthrift_MyService_adapted_param_result()
        

        return serialize_iobuf(return_struct, protocol)

