#
# Autogenerated by Thrift for thrift/compiler/test/fixtures/qualified/src/module2.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#

from libcpp.memory cimport shared_ptr

cimport module2.types as _fbthrift_ctypes


cdef shared_ptr[_fbthrift_ctypes.cStruct] Struct_convert_to_cpp(object inst) except*
cdef object Struct_from_cpp(const shared_ptr[_fbthrift_ctypes.cStruct]& c_struct)

cdef shared_ptr[_fbthrift_ctypes.cBigStruct] BigStruct_convert_to_cpp(object inst) except*
cdef object BigStruct_from_cpp(const shared_ptr[_fbthrift_ctypes.cBigStruct]& c_struct)

