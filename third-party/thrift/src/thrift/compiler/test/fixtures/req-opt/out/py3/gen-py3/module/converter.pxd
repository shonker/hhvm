#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/req-opt/src/module.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#

from libcpp.memory cimport shared_ptr

cimport module.types as _fbthrift_ctypes


cdef shared_ptr[_fbthrift_ctypes.cFoo] Foo_convert_to_cpp(object inst) except*
cdef object Foo_from_cpp(const shared_ptr[_fbthrift_ctypes.cFoo]& c_struct)

