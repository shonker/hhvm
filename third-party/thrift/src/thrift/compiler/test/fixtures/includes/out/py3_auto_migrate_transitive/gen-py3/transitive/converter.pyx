
#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/includes/src/transitive.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#

from libcpp.memory cimport make_shared
from thrift.python.capi.cpp_converter cimport cpp_to_python, python_to_cpp
from cython.operator cimport dereference as deref

cdef extern from "thrift/compiler/test/fixtures/includes/gen-python-capi/transitive/thrift_types_capi.h":
    pass

cdef shared_ptr[_fbthrift_ctypes.cFoo] Foo_convert_to_cpp(object inst) except*:
    return make_shared[_fbthrift_ctypes.cFoo](python_to_cpp[_fbthrift_ctypes.cFoo](inst))


cdef object Foo_from_cpp(const shared_ptr[_fbthrift_ctypes.cFoo]& c_struct):
    return cpp_to_python[_fbthrift_ctypes.cFoo](deref(c_struct))
