# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cimport cython

from libcpp.memory cimport make_unique

from cpython.list cimport PyList_New, PyList_SET_ITEM
from cpython.ref cimport Py_INCREF
from cpython.set cimport PySet_New, PySet_Add

from cython.operator cimport dereference as deref

from thrift.python.mutable_containers cimport (
    MutableList,
    MutableSet,
    MutableMap,
)
from thrift.python.mutable_types cimport (
    MutableStruct,
    MutableStructInfo,
    MutableUnion,
    MutableUnionInfo,
)
from thrift.python.exceptions cimport GeneratedError
from thrift.python.types cimport getCTypeInfo


@cython.final
cdef class MutableStructTypeInfo(TypeInfoBase):
    """
    `MutableStructTypeInfo` is similar to `StructTypeInfo`. However, we had to
    make a copy because mutable and immutable types inherit from different base
    classes, `MutableStruct` and `Struct`, respectively.
    """
    def __cinit__(self, mutable_struct_class):
        self._mutable_struct_class = mutable_struct_class
        py_mutable_struct_info = mutable_struct_class._fbthrift_mutable_struct_info

        cdef cDynamicStructInfo* c_struct_info
        if isinstance(py_mutable_struct_info, MutableUnionInfo):
            c_struct_info = (<MutableUnionInfo>py_mutable_struct_info).cpp_obj.get()
        else:
            c_struct_info = (<MutableStructInfo>py_mutable_struct_info).cpp_obj.get()

        self.cpp_obj = createImmutableStructTypeInfo(deref(c_struct_info))

    cdef const cTypeInfo* get_cTypeInfo(self):
        return &self.cpp_obj

    cdef to_internal_data(self, object value):
        """
        Validates and converts the given (mutable struct) `value` to a format
        that the serializer can understand.

        Args:
            value: should be an instance of `self._mutable_struct_class`, Otherwise, raises `TypeError`.

        Returns: The "mutable struct tuple" of the given value (see `createMutableStructTupleWithDefaultValues()`).

        Raises:
            TypeError if `value` is not an instance of `self._mutable_struct_class`
        """
        if not isinstance(value, self._mutable_struct_class):
            raise TypeError(f"value {value} is not a {self._mutable_struct_class !r}, is actually of type {type(value)}.")

        if isinstance(value, MutableStruct):
            return (<MutableStruct>value)._fbthrift_data
        if isinstance(value, MutableUnion):
            return (<MutableUnion>value)._fbthrift_data
        if isinstance(value, GeneratedError):
            return (<GeneratedError>value)._fbthrift_data

        raise TypeError(f"MutableStructInfo cannot convert {self._mutable_struct_class} to internal data")

    # convert deserialized data to user format
    cdef to_python_value(self, object struct_tuple):
        return self._mutable_struct_class._fbthrift_create(struct_tuple)

    def to_container_value(self, object value not None):
        """
        A type checker, called from the Immutable containers, allows mutable
        types to be part of the process during the constant definition.
        """
        if not isinstance(value, self._mutable_struct_class):
            raise TypeError(f"value {value} is not a {self._mutable_struct_class !r}, is actually of type {type(value)}.")

        return value

    def same_as(MutableStructTypeInfo self, other):
        if other is self:
            return True

        if not isinstance(other, MutableStructTypeInfo):
            return False

        return self._mutable_struct_class == (<MutableStructTypeInfo>other)._mutable_struct_class


@cython.final
cdef class MutableListTypeInfo(TypeInfoBase):
    """
    `MutableListTypeInfo` is similar to `ListTypeInfo`. However, the main
    difference is that `MutableListTypeInfo` uses a Python `list` for internal
    data representation, whereas `ListTypeInfo` uses a Python `tuple` for
    internal data representation. In addition, `MutableListTypeInfo` returns
    a `MutableList` from the `to_python_value()` method, rather than a `List`.
    """
    def __cinit__(self, val_info):
        self.val_info = val_info
        self.cpp_obj = make_unique[cMutableListTypeInfo](getCTypeInfo(val_info))

    cdef const cTypeInfo* get_cTypeInfo(self):
        return self.cpp_obj.get().get()

    cdef to_internal_data(self, object values):
        """
        Validates the `values` and converts them into an internal data representation.

        Args:
            values (iterable): An iterable object. Each value in the iteration should
            be of a valid value type as verified by in `self._val_info`.

        Returns a Python list with converted values, representing the internal data
        """
        cdef TypeInfoBase val_type_info = self.val_info
        cdef int idx = 0
        cdef list lst = PyList_New(len(values))
        for idx, value in enumerate(values):
            internal_data = val_type_info.to_internal_data(value)
            Py_INCREF(internal_data)
            PyList_SET_ITEM(lst, idx, internal_data)

        return lst

    cdef to_python_value(self, object value):
        """
        Converts the given internal data (`value`) into a `MutableList` The resulting
        `MutableList` is capable of converting the internal data to Python values
        for its elements.

        Args:
            value (object): A Python list, very likely returned by the
            `MutableListInfo.to_internal_data()` method.

        Returns a `MutableList` instance with the internal data (`value`) and
            the type info (`self.val_info`) attached.
        """
        return MutableList(self.val_info, value)

    def to_container_value(MutableListTypeInfo self, object value not None):
        """
        A type checker, called from the Immutable containers, allows mutable
        types to be part of the process during the constant definition.

        Validates the `value` and returns a `MutableList`
        """
        if isinstance(value, MutableList):
            return value

        return (<TypeInfoBase>self).to_python_value(
            (<TypeInfoBase>self).to_internal_data(value)
        )

    def same_as(MutableListTypeInfo self, other):
        if other is self:
            return True

        if not isinstance(other, MutableListTypeInfo):
            return False

        return self.val_info.same_as((<MutableListTypeInfo>other).val_info)


@cython.final
cdef class MutableSetTypeInfo(TypeInfoBase):
    """
    `MutableSetTypeInfo` is similar to `SetTypeInfo`. However, the main
    difference is that `MutableSetTypeInfo` uses a Python `set` for internal
    data representation, whereas `SetTypeInfo` uses a Python `frozenset` for
    internal data representation. In addition, `MutableSetTypeInfo` returns
    a `MutableSet` from the `to_python_value()` method, rather than a `Set`.
    """
    def __cinit__(self, val_info):
        self.val_info = val_info
        self.cpp_obj = make_unique[cMutableSetTypeInfo](getCTypeInfo(val_info))

    cdef const cTypeInfo* get_cTypeInfo(self):
        return self.cpp_obj.get().get()

    cdef to_internal_data(self, object values):
        """
        Validates the `values` and converts them into an internal data representation.

        Args:
            values (iterable): An iterable object. Each value in the iteration should
            be of a valid value type as verified by in `self._val_info`.

        Returns a Python set with converted values, representing the internal data
        """
        cdef set py_set = PySet_New(<object>NULL)
        cdef TypeInfoBase val_type_info = self.val_info
        for value in values:
            PySet_Add(py_set, val_type_info.to_internal_data(value))

        return py_set

    cdef to_python_value(self, object value):
        """
        Converts the given internal data (`value`) into a `MutableSet` The
        resulting `MutableSet` is capable of converting the internal data to
        Python values for its elements.

        Args:
            value (object): A Python set, very likely returned by the
            `MutableSetInfo.to_internal_data()` method.

        Returns a `MutableSet` instance with the internal data (`value`) and
            the type info (`self.val_info`) attached.
        """
        return MutableSet(self.val_info, value)

    def to_container_value(MutableSetTypeInfo self, object value not None):
        """
        A type checker, called from the Immutable containers, allows mutable
        types to be part of the process during the constant definition.

        Validates the `value` and returns a `MutableSet`
        """
        if isinstance(value, MutableSet):
            return value

        return (<TypeInfoBase>self).to_python_value(
            (<TypeInfoBase>self).to_internal_data(value)
        )

    def same_as(MutableSetTypeInfo self, other):
        if other is self:
            return True

        if not isinstance(other, MutableSetTypeInfo):
            return False

        return self.val_info.same_as((<MutableSetTypeInfo>other).val_info)


@cython.final
cdef class MutableMapTypeInfo(TypeInfoBase):
    def __cinit__(self, key_info, val_info):
        self.key_info = key_info
        self.val_info = val_info
        self.cpp_obj = make_unique[cMutableMapTypeInfo](
            getCTypeInfo(key_info),
            getCTypeInfo(val_info),
        )

    cdef const cTypeInfo* get_cTypeInfo(self):
        return self.cpp_obj.get().get()

    cdef to_internal_data(self, object values):
        """
        Validates the `values` and converts them into an internal data representation.

        Args:
            values (iterable): An iterable object. Each key, value pair in the iteration
            should be of a valid key and valid value type as verified by in `self.key_info`
            and `self.val_info`.

        Returns a Python dict with converted values, representing the internal data
        """
        if values is None:
            raise TypeError("Argument 'value' must not be None")

        cdef TypeInfoBase key_type_info = self.key_info
        cdef TypeInfoBase val_type_info = self.val_info
        return {
            key_type_info.to_internal_data(k): val_type_info.to_internal_data(v) for k, v in values.items()
        }

    cdef to_python_value(self, object value):
        """
        Converts the given internal data (`value`) into a `MutableMap` The
        resulting `MutableMap` is capable of converting the internal data to
        Python values for its items.

        Args:
            value (object): A Python dict, very likely returned by the
            `MutableMapTypeInfo.to_internal_data()` method. The types of all
            items in this dictionary MUST match `key_info` and `val_info`.

        Returns a `MutableMap` instance with the internal data (`value`) and
            the type infos (`self.key_info` and `self.val_info`) attached.
        """
        return MutableMap(self.key_info, self.val_info, value)

    def to_container_value(self, object value not None):
        """
        A type checker, called from the Immutable containers, allows mutable
        types to be part of the process during the constant definition.

        Validates the `value` and returns a `MutableMap`
        """
        if isinstance(value, MutableMap):
            return value

        return (<TypeInfoBase>self).to_python_value(
            (<TypeInfoBase>self).to_internal_data(value)
        )

    def same_as(MutableMapTypeInfo self, other):
        if other is self:
            return True

        if not isinstance(other, MutableMapTypeInfo):
            return False

        return (self.key_info.same_as((<MutableMapTypeInfo>other).key_info) and
            self.val_info.same_as((<MutableMapTypeInfo>other).val_info))
