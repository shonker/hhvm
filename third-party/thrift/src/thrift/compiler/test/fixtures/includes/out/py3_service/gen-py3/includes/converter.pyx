
#
# Autogenerated by Thrift for includes.thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#



cdef shared_ptr[_fbthrift_ctypes.cIncluded] Included_convert_to_cpp(object inst) except*:
    return (<_fbthrift_ctypes.Included?>inst)._cpp_obj_FBTHRIFT_ONLY_DO_NOT_USE


cdef object Included_from_cpp(const shared_ptr[_fbthrift_ctypes.cIncluded]& c_struct):
    return _fbthrift_ctypes.Included._create_FBTHRIFT_ONLY_DO_NOT_USE(c_struct)
