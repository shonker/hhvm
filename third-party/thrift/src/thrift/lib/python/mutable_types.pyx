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

from libcpp.memory cimport make_unique
from libcpp.utility cimport move as std_move
from folly.iobuf cimport from_unique_ptr

from cpython.object cimport PyCallable_Check
from cpython.ref cimport Py_INCREF, Py_DECREF
from cpython.tuple cimport PyTuple_New, PyTuple_SET_ITEM
from cpython.unicode cimport PyUnicode_AsUTF8String

import enum

from thrift.python.mutable_serializer cimport cserialize, cdeserialize
from thrift.python.mutable_typeinfos cimport (
    MutableListTypeInfo,
    MutableSetTypeInfo,
    MutableMapTypeInfo,
)
from thrift.python.types cimport (
    AdaptedTypeInfo,
    FieldInfo,
    StringTypeInfo,
    TypeInfoBase,
    getCTypeInfo,
    set_struct_field,
    _fbthrift_compare_struct_less,
)

from cython.operator cimport dereference as deref

cdef extern from "<Python.h>":
    cdef const char * PyUnicode_AsUTF8(object unicode)


def fill_specs(*structured_thrift_classes):
    """
    Completes the initialization of the given Thrift-generated Struct (and
    Union) classes.

    This is called at the end of the modules that define the corresponding
    generated types (i.e., the `thrift_types.py` files), after the given classes
    have been created but not yet fully initialized. It provides support for
    dependent classes being defined in arbitrary order.

    If struct A has a field of type struct B, but the generated class A is
    defined before B, we are not able to populate the specs for A as part of the
    class creation, hence this call.

    Args:
        *structured_thrift_classes: Sequence of class objects, each one of which
            corresponds to a `MutableStruct` (i.e., created by/instance of
            `MutableStructMeta`) or a `MutableUnion` (i.e., created by/instance
            of `MutableUnionMeta`).
    """

    for cls in structured_thrift_classes:
        cls._fbthrift_fill_spec()

    for cls in structured_thrift_classes:
        if not isinstance(cls, MutableUnionMeta):
            cls._fbthrift_store_field_values()


class _MutableStructField:
    """
    The `_MutableStructField` class is a descriptor class that is used to
    manage the access to a specific field in a mutable Thrift struct object.
    It uses the descriptor protocol.
    """
    # `_field_index` is the insertion order of the field in the
    # `MutableStructInfo` (this is not the Thrift field id)
    __slots__ = ('_field_index',)

    def __init__(self, field_id):
        self._field_index = field_id

    def __get__(self, obj, objtype):
        return (<MutableStruct>obj)._fbthrift_get_field_value(self._field_index)

    def __set__(self, obj, value):
        (<MutableStruct>obj)._fbthrift_set_field_value(self._field_index, value)


cdef is_cacheable_non_primitive(ThriftIdlType idl_type):
    return idl_type in (ThriftIdlType.String, ThriftIdlType.Struct)


class _MutableStructCachedField:
    __slots__ = ('_field_index')

    def __init__(self, field_id):
        self._field_index = field_id

    def __get__(self, obj, objtype):
        return (<MutableStruct>obj)._fbthrift_get_cached_field_value(self._field_index)

    def __set__(self, obj, value):
        (<MutableStruct>obj)._fbthrift_set_field_value(self._field_index, value)
        obj._fbthrift_field_cache[self._field_index] = None


cdef is_container(ThriftIdlType idl_type):
    return idl_type in (ThriftIdlType.List, ThriftIdlType.Set, ThriftIdlType.Map)

class _MutableStructContainerField:
    __slots__ = ('_field_index')

    def __init__(self, field_id):
        self._field_index = field_id

    def __get__(self, obj, objtype):
        return (<MutableStruct>obj)._fbthrift_get_cached_field_value(self._field_index)

    def __set__(self, obj, value):
        raise TypeError(
                "Thrift container types do not support direct assignment."
                " However, you can add, remove or update items within the"
                " container")


cdef class MutableStructOrUnion:
    cdef IOBuf _fbthrift_serialize(self, Protocol proto):
        raise NotImplementedError("Not implemented on base MutableStructOrUnion class")
    cdef uint32_t _fbthrift_deserialize(self, IOBuf buf, Protocol proto) except? 0:
        raise NotImplementedError("Not implemented on base MutableStructOrUnion class")
    cdef _fbthrift_get_field_value(self, int16_t index):
        raise NotImplementedError("Not implemented on base MutableStructOrUnion class")


cdef class MutableStruct(MutableStructOrUnion):
    """
    Base class for all generated (mutable) classes corresponding to Thrift
    struct in thrift-python.

    Instance variables:
        _fbthrift_data: "mutable struct tuple" that holds the "isset" flag array and
            values for all fields. See `createMutableStructTupleWithDefaultValues()`.

        _fbthrift_field_cache: This is a list that stores instances of a field's
            Python value. It is especially useful when creating a Python value is
            relatively expensive, such as when calling the `TypeInfo.to_python_value()`
            method. For example, in the case of adapted types, we store the Python
            value in this list to avoid repeated calls to the adapter class.
            This list also stores instances when we want to return the same instance
            multiple times. For instance, if a struct field is a Thrift `list`, we store
            the `MutableList` instance in this list. This allows us to return the same
            `MutableList` instance for all attribute accesses.
    """

    def __cinit__(self, **kwargs):
        """
        Args:
            **kwargs: names and values of the Thrift fields to set for this
                 instance. All names must match declared fields of this Thrift
                 Struct (or a `TypeError` will be raised). Values are in
                 "Python value" representation, as opposed to "internal data"
                 representation (see `*TypeInfo` classes).
        """
        self._initStructTupleWithValues(kwargs)
        cdef MutableStructInfo mutable_struct_info = type(self)._fbthrift_mutable_struct_info
        self._fbthrift_field_cache = [None] * len(mutable_struct_info.fields)

    def __init__(self, **kwargs):
        pass

    cdef _initStructTupleWithValues(self, kwargs) except *:
        cdef MutableStructInfo mutable_struct_info = self._fbthrift_mutable_struct_info

        # If no keyword arguments are provided, initialize the Struct with
        # default values.
        if not kwargs:
            self._fbthrift_data = createMutableStructTupleWithDefaultValues(mutable_struct_info.cpp_obj.get().getStructInfo())
            return

        # Instantiate a tuple with 'None' values, then assign the provided
        # keyword arguments to the respective fields.
        self._fbthrift_data = createStructTupleWithNones(mutable_struct_info.cpp_obj.get().getStructInfo())
        for name, value in kwargs.items():
            field_index = mutable_struct_info.name_to_index.get(name)
            if field_index is None:
                raise TypeError(f"{self.__class__.__name__}.__init__() got an unexpected keyword argument '{name}'")

            self._fbthrift_set_field_value(field_index, value)

        # If any fields remain unset, initialize them with their respective
        # default values.
        populateMutableStructTupleUnsetFieldsWithDefaultValues(
                self._fbthrift_data,
                mutable_struct_info.cpp_obj.get().getStructInfo()
        )

    cdef _fbthrift_set_field_value(self, int16_t index, object value):
        cdef MutableStructInfo mutable_struct_info = self._fbthrift_mutable_struct_info
        cdef FieldInfo field_info = mutable_struct_info.fields[index]

        if field_info.adapter_info is not None:
            adapter_class, transitive_annotation = field_info.adapter_info
            value = adapter_class.to_thrift_field(
                value,
                field_info.id,
                self,
                transitive_annotation=transitive_annotation(),
            )

        set_struct_field(
            self._fbthrift_data,
            index,
            (<TypeInfoBase>mutable_struct_info.type_infos[index]).to_internal_data(value),
        )

    cdef _fbthrift_get_field_value(self, int16_t index):
        cdef MutableStructInfo mutable_struct_info = self._fbthrift_mutable_struct_info
        cdef TypeInfoBase field_type_info = mutable_struct_info.type_infos[index]
        cdef FieldInfo field_info = mutable_struct_info.fields[index]

        data = self._fbthrift_data[index + 1]
        if field_info.adapter_info is not None:
            py_value = field_type_info.to_python_value(data)
            adapter_class, transitive_annotation = field_info.adapter_info
            return adapter_class.from_thrift_field(
                py_value,
                field_info.id,
                self,
                transitive_annotation=transitive_annotation(),
            )

        return field_type_info.to_python_value(data) if data is not None else None

    cdef _fbthrift_get_cached_field_value(MutableStruct self, int16_t index):
        cached = self._fbthrift_field_cache[index]
        if cached is not None:
            return cached

        value = self._fbthrift_get_field_value(index)
        self._fbthrift_field_cache[index] = value
        return value

    def __eq__(MutableStruct self, other):
        if other is self:
            return True

        if type(other) != type(self):
            return False

        for name, value in self:
            if value != getattr(other, name):
                return False

        return True

    def __lt__(self, other):
        return _fbthrift_compare_struct_less(self, other, False)

    def __le__(self, other):
        return _fbthrift_compare_struct_less(self, other, True)

    def __iter__(self):
        cdef MutableStructInfo info = self._fbthrift_mutable_struct_info
        for name in info.name_to_index:
            yield name, getattr(self, name)

    cdef _fbthrift_reset_field_to_standard_default(self, int16_t index):
        cdef MutableStructInfo mutable_struct_info = self._fbthrift_mutable_struct_info
        resetFieldToStandardDefault(
            self._fbthrift_data,
            mutable_struct_info.cpp_obj.get().getStructInfo(),
            index,
        )

    cdef IOBuf _fbthrift_serialize(self, Protocol proto):
        cdef MutableStructInfo info = self._fbthrift_mutable_struct_info
        return from_unique_ptr(
            std_move(cserialize(deref(info.cpp_obj), self._fbthrift_data, proto))
        )

    cdef uint32_t _fbthrift_deserialize(self, IOBuf buf, Protocol proto) except? 0:
        cdef MutableStructInfo info = self._fbthrift_mutable_struct_info
        cdef uint32_t lenght = cdeserialize(deref(info.cpp_obj), buf._this, self._fbthrift_data, proto)
        return lenght

    def _do_not_use_resetFieldToStandardDefault(self, field_name: str):
        """
        This method is currently intended for internal testing purposes only.
        Please avoid using it as it will be replaced with a more appropriate
        mechanism in the future
        """
        cdef MutableStructInfo mutable_struct_info = self._fbthrift_mutable_struct_info
        field_index = mutable_struct_info.name_to_index.get(field_name)
        if field_index is None:
            raise TypeError(f"got an unexpected field_name: '{field_name}'")
        self._fbthrift_reset_field_to_standard_default(field_index)
        self._fbthrift_field_cache[field_index] = None

    @classmethod
    def _fbthrift_create(cls, data):
        cdef MutableStruct inst = cls.__new__(cls)
        inst._fbthrift_data = data
        return inst


cdef class MutableStructInfo:
    """
    Stores information for a specific Thrift Struct class.

    Instance Variables:
        fields: tuple[FieldInfo, ...] (from `_fbthrift_SPEC`).

        cpp_obj: cDynamicStructInfo for this struct.

        type_infos: Tuple whose size matches the number of fields in the Thrift
            struct. Initialized by calling `fill()`.

        name_to_index: Dict[str (field name), int (index in `fields`).].
            Initialized by calling `fill()`.
    """

    def __cinit__(self, name: str, fields: tuple[FieldInfo, ...]):
        """
        Stores information for a Thrift Struct class with the given name.

        Args:
            name (str): Name of the Thrift Struct (as specified in IDL)
            fields (Set[Tuple]): Field spec tuples. See class docstring above.
        """
        self.fields = fields
        cdef int16_t num_fields = len(fields)
        self.cpp_obj = make_unique[cDynamicStructInfo](
            PyUnicode_AsUTF8(name),
            num_fields,
            False, # isUnion
        )
        self.type_infos = PyTuple_New(num_fields)
        self.name_to_index = {}

    cdef void fill(self) except *:
        """
        Completes the initialization of this instance by populating all
        information relative to this Struct's fields.

        Must be called exactly once, after `__cinit__()` but before any other
        method.

        Upon successful return, the following attributes are fully initialized:
          - `self.type_infos`
          - `self.name_to_index`
          - field infos in the `self.cpp_obj` (see
            `DynamicStructInfo::addFieldInfo()`)
        """

        cdef cDynamicStructInfo* dynamic_struct_info = self.cpp_obj.get()
        type_infos = self.type_infos
        for idx, field_info in enumerate(self.fields):
            # field_type_info can be a lambda function so types with dependencies
            # won't need to be defined in order, see class docstring above.
            field_type_info = field_info.type_info
            if PyCallable_Check(field_type_info):
                field_type_info = field_type_info()

            # The rest of the code assumes that all the `TypeInfo` classes extend
            # from `TypeInfoBase`. Instances are typecast to `TypeInfoBase` before
            # the `to_internal_data()` and `to_python_value()` methods are called.
            if not isinstance(field_type_info, TypeInfoBase):
                raise TypeError(f"{type(field_type_info).__name__} is not subclass of TypeInfoBase.")

            Py_INCREF(field_type_info)
            PyTuple_SET_ITEM(type_infos, idx, field_type_info)
            field_info.type_info = field_type_info
            self.name_to_index[field_info.py_name] = idx
            dynamic_struct_info.addFieldInfo(
                field_info.id,
                field_info.qualifier,
                PyUnicode_AsUTF8(field_info.name),
                getCTypeInfo(field_type_info),
            )

    cdef void _initialize_default_values(self) except *:
        """
        Initializes the default values of fields in this Struct.

        Upon successful return, the field value(s) in `self.cpp_obj` are
        iniitalized (see `DynamicStructInfo::addFieldValue()`).
        """
        cdef cDynamicStructInfo* dynamic_struct_info = self.cpp_obj.get()
        for idx, field in enumerate(self.fields):
            default_value = field.default_value
            if default_value is None:
                continue
            if callable(default_value):
                default_value = default_value()

            type_info = self.type_infos[idx]
            if isinstance(type_info, AdaptedTypeInfo):
                type_info = (<AdaptedTypeInfo>type_info)._orig_type_info

            default_value = (<TypeInfoBase>type_info).to_internal_data(default_value)
            dynamic_struct_info.addFieldValue(idx, default_value)


class MutableStructMeta(type):
    """Metaclass for all generated (mutable) thrift-python Struct types."""

    def __new__(cls, cls_name, bases, dct):
        """
        Returns a new Thrift Struct class with the given name and members.

        Args:
            cls_name (str): Name of the Thrift Struct, as specified in the
                Thrift IDL.
            bases: Unused, expected to always be empty. If not, this method
                will raise a `TypeError`.
            dct (Dict): Class members, including the SPEC for this class under
                the key '_fbthrift_SPEC'.

        Returns:
            A new class, with the given `cls_name`, corresponding to a Thrift
            Struct. The returned class inherits from `MutableStruct` and
            provides properties for all primitive and non-primitive fields
            (including any adapted fields) specified in the SPEC.

            The returned class will also have the following additional class
            attribute, meant for internal (Thrift) processing:

                _fbthrift_mutable_struct_info: MutableStructInfo
        """
        if bases:
            raise TypeError("Inheriting from thrift-python data types is forbidden: "
                           f"'{cls_name}' cannot inherit from '{bases[0].__name__}'")

        # Set[Tuple (field spec)]. See `MutableStructInfo` class docstring for the
        # contents of the field spec tuples.
        fields = dct.pop('_fbthrift_SPEC')
        dct["_fbthrift_mutable_struct_info"] = MutableStructInfo(cls_name, fields)

        # List[Tuple[int (index in fields), str (field name), int (IDL Type)]
        primitive_types = []
        # List[Tuple[int (index in fields), str (field name), int (IDL Type), Optional[AdapterInfo]]
        non_primitive_types = []

        slots = ['_fbthrift_field_cache']
        for field_index, field_info in enumerate(fields):
            slots.append(field_info.py_name)

            # if field has an adapter or is not primitive type, consider
            # as "non-primitive"
            if field_info.adapter_info is not None or not field_info.is_primitive:
                non_primitive_types.append(
                    (field_index, field_info.py_name, field_info.idl_type, field_info.adapter_info))
            else:
                primitive_types.append((field_index, field_info.py_name, field_info.idl_type))

        dct["__slots__"] = slots
        klass = super().__new__(cls, cls_name, (MutableStruct,), dct)

        for field_index, field_name, *_ in primitive_types:
            type.__setattr__(
                klass,
                field_name,
                _MutableStructField(field_index),
            )

        # DO_BEFORE(alperyoney,20240515): Implement descriptor for non-primitive
        # mutable types.
        # For now, handle non-primitive-types similarly to primitive types and
        # use the `_MutableStructField` descriptor for all types, except for
        # `list`. For `list`, use `_MutableStructContainerField` descriptor.
        for field_index, field_name, idl_type, adapter_info in non_primitive_types:
            field_descriptor = _MutableStructField(field_index)
            if is_container(idl_type):
                field_descriptor = _MutableStructContainerField(field_index)
            elif adapter_info is not None or is_cacheable_non_primitive(idl_type):
                field_descriptor = _MutableStructCachedField(field_index)

            type.__setattr__(
                klass,
                field_name,
                field_descriptor,
            )

        return klass

    def _fbthrift_fill_spec(cls):
        """
        Completes the initialization of all specs for this Struct class.

        This should be called once, after all generated classes (including
        unions and structs) for a given module have been created.
        """
        (<MutableStructInfo>cls._fbthrift_mutable_struct_info).fill()

    def _fbthrift_store_field_values(cls):
        """
        Initializes the default values of fields (if any) for this Struct.

        This should be called once, after `_fbthrift_fill_spec()` has been
        called for all generated classes (including unions and structs) in
        a module.
        """
        (<MutableStructInfo>cls._fbthrift_mutable_struct_info)._initialize_default_values()

    def __iter__(cls):
        """
        Iterating over Thrift-generated Struct classes yields the names of the
        fields in the struct.

        Should not be called prior to `_fbthrift_fill_spec()`.
        """
        cdef MutableStructInfo mutable_struct_info = cls._fbthrift_mutable_struct_info
        for name in mutable_struct_info.name_to_index.keys():
            yield name, None


cdef class MutableUnionInfo:
    def __cinit__(self, union_name: str, field_infos: tuple[FieldInfo, ...]):
        self.fields = field_infos
        self.cpp_obj = make_unique[cDynamicStructInfo](
            PyUnicode_AsUTF8(union_name),
            len(field_infos),
            True, # isUnion
        )
        self.type_infos = {}
        self.id_to_adapter_info = {}
        self.name_to_index = {}

    cdef void _fill_mutable_union_info(self) except *:
        cdef cDynamicStructInfo* dynamic_struct_info = self.cpp_obj.get()
        for idx, field_info in enumerate(self.fields):
            # type_info can be a lambda function so types with dependencies
            # won't need to be defined in order
            if callable(field_info.type_info):
                field_info.type_info = field_info.type_info()
            self.type_infos[field_info.id] = field_info.type_info
            self.id_to_adapter_info[field_info.id] = field_info.adapter_info
            self.name_to_index[field_info.py_name] = idx
            dynamic_struct_info.addFieldInfo(
                field_info.id,
                field_info.qualifier,
                PyUnicode_AsUTF8(field_info.name),
                getCTypeInfo(field_info.type_info)
            )

cdef tuple _validate_union_init_kwargs(object mutable_union_class, dict kwargs):
    """
    Validates the given Thrift union initialization keyword arguments and returns the
    data needed to set the corresponding field (if any).

    Returns: tuple[field_enum, field_value], where:
        `field_enum` corresponds to the field being initialized, and must be one of the
        values in the `FbThriftUnionFieldEnum` enumeration type of the given
        `mutable_union_class`. If no field is being initialized (i.e., the Thrift union
        is empty), the `FBTHRIFT_UNION_EMPTY` member of that enumeration type is
        returned.

        `field_value` holds the value specified for the corresponding field, in "python
        value" format (as opposed to "internal data", see `TypeInfoBase`). If no field
        is specified (see `field_enum` above), then this value is `None`.

    Raises: Exception if the given keyword arguments are invalid.
    """

    current_field_name = None
    current_field_value = None
    for field_name, field_value in kwargs.items():
        if field_value is None:
            continue

        if current_field_name is not None:
            raise RuntimeError(
                f"Cannot initialize Thrift union ({mutable_union_class.__name__}) with "
                f"more than one keyword argument (got non-None value for {field_name}, "
                f"but already had one for {current_field_name})."
            )

        current_field_name = field_name
        current_field_value = field_value

    fields_enum_type = mutable_union_class.FbThriftUnionFieldEnum
    if current_field_name is None:
        assert current_field_value is None
        return (fields_enum_type.FBTHRIFT_UNION_EMPTY, None)

    try:
        field_enum = fields_enum_type[current_field_name]
    except KeyError as e:
        raise RuntimeError(
            f"Cannot initialize Thrift union ({mutable_union_class.__name__}): unknown "
            f"field ({current_field_name})."
        ) from e
    else:
        return (field_enum, current_field_value)


cdef class MutableUnion(MutableStructOrUnion):
    def __cinit__(self):
        self._fbthrift_data = createUnionTuple()

    def __init__(self, **kwargs):
        field_enum, field_python_value = _validate_union_init_kwargs(
            type(self), kwargs
        )
        cdef int field_id = field_enum.value

        # If no field is specified, exit early.
        if field_id == 0:
            self._fbthrift_update_current_field_attributes()
            return

        self._fbthrift_set_mutable_union_value(field_id, field_python_value)

    cdef void _fbthrift_set_mutable_union_value(
        self, int field_id, object field_python_value
    ) except *:
        """
        Updates this union to hold the given value (corresponding to the given field).

        Args:
            field_id: Thrift field ID of the field being set (or 0 to "clear" this union
                and mark it as empty).
            field_python_value: Value for the given field, in "python value" format (as
                opposed to "internal data", see `TypeInfoBase`). If `field_id` is 0,
                this must be `None`.
        """
        field_internal_value = (
            self._fbthrift_convert_field_python_value_to_internal_data(
                field_id, field_python_value
            )
        )

        Py_INCREF(field_id)
        old_field_id = self._fbthrift_data[0]
        PyTuple_SET_ITEM(self._fbthrift_data, 0, field_id)
        Py_DECREF(old_field_id)

        old_value = self._fbthrift_data[1]
        Py_INCREF(field_internal_value)
        PyTuple_SET_ITEM(self._fbthrift_data, 1, field_internal_value)
        Py_DECREF(old_value)

        self._fbthrift_update_current_field_attributes()

    cdef object _fbthrift_convert_field_python_value_to_internal_data(
        self, int field_id, object field_python_value
    ):
        """
        Converts the given python value to its "internal data" representation, assuming
        it is a value for a field of this Thrift union with id `field_id`.

        If `field_id` is the special value 0 (corresponding to the case where the Thrift
        union is "empty", i.e. does not have any field set), `field_python_value` MUST
        be `None` and the returned value will always be `None`.

        Raises:
            AssertionError if `field_id` is 0 (i.e., FBTHRIFT_UNION_EMPTY) but
            `field_python_value` is not `None`.

            Exception if the operation cannot be completely successfully.
        """
        if field_id == 0:
            assert field_python_value is None
            return None

        cdef MutableUnionInfo union_info = <MutableUnionInfo>(
            type(self)._fbthrift_mutable_struct_info
        )

        adapter_info = union_info.id_to_adapter_info[field_id]
        if adapter_info:
            adapter_class, transitive_annotation = adapter_info
            field_python_value = adapter_class.to_thrift_field(
                field_python_value,
                field_id,
                self,
                transitive_annotation=transitive_annotation(),
            )

        cdef TypeInfoBase type_info = <TypeInfoBase>union_info.type_infos[field_id]
        return type_info.to_internal_data(field_python_value)

    cdef void _fbthrift_update_current_field_attributes(self) except *:
        """
        Updates the `fbthrift_current_*` attributes of this instance with the
        information of the current field for this union (or the corresponding field enum
        value if this union is empty).
        """
        cdef int current_field_enum_value = self._fbthrift_data[0]
        self.fbthrift_current_field  = type(self).FbThriftUnionFieldEnum(
            current_field_enum_value
        )
        self.fbthrift_current_value = self._fbthrift_get_current_field_python_value(
            current_field_enum_value
        )

    cdef object _fbthrift_get_current_field_python_value(
        self, int current_field_enum_value
    ):
        """
        Returns the current value for this union, in "python" format (as opposed to
        "internal data", see `TypeInfoBase`).

        Args:
            current_field_enum_value: the field ID of the current field, read from
                `self._fbthrift_data[0]` and passed in to avoid unnecessarily reading it
                again.
        """
        field_internal_data = self._fbthrift_data[1]

        if current_field_enum_value == 0:
            assert field_internal_data is None

        # DO_BEFORE(aristidis,20240701): Determine logic for accessing adapted fields
        # in unions (there seems to be a discrepancy with structs, see 
        # `MutableStruct._fbthrift_get_field_value()`).
        if field_internal_data is None:
            return None

        cdef MutableUnionInfo union_info = (
            <MutableUnionInfo>type(self)._fbthrift_mutable_struct_info
        )
        cdef TypeInfoBase type_info = (
            <TypeInfoBase>union_info.type_infos[current_field_enum_value]
        )
        return type_info.to_python_value(field_internal_data)


    cdef object _fbthrift_get_field_value(self, int16_t field_id):
        """
        Returns the value of the field with the given `field_id` if it is indeed the
        field that is (currently) set for this union.

        Raises:
            ValueError if `field_id` does not correspond to a valid field id for this
                Thrift union.

            AttributeError if this union does not currently hold a value for the given
                `field_id` (i.e., it either holds a value for another field, or is
                empty).
        """
        current_field_enum = self.fbthrift_current_field
        cdef int current_field_enum_value = current_field_enum.value
        if (current_field_enum_value == field_id):
            return self.fbthrift_current_value

        # ERROR: Requested field_id does not match current field.
        union_class = type(self)
        requested_field_enum = union_class.FbThriftUnionFieldEnum(field_id)
        raise AttributeError(
            f"Error retrieving Thrift union ({union_class.__name__}) field: requested "
            f"'{requested_field_enum.name}', but currently holds "
            f"'{current_field_enum.name}'."
        )

    @classmethod
    def _fbthrift_create(cls, data):
        cdef MutableUnion inst = cls.__new__(cls)
        inst._fbthrift_data = data
        return inst


def _gen_mutable_union_field_enum_members(field_infos):
    yield ("FBTHRIFT_UNION_EMPTY", 0)
    for f in field_infos:
        yield (f.py_name, f.id)


cdef _make_fget_union(field_id, adapter_info):
    """
    Returns a function that takes a `Union` instance and returns the value of the field
    with the given `field_id`.

    If `adapter_info` is not None, the corresponding adapter will be called with the
    field value prior to returning.

    Args:
        field_id (int)
        adapter_info (typing.Optional[object])
    """
    if adapter_info:
        adapter_class, transitive_annotation = adapter_info
        return property(
            lambda self: adapter_class.from_thrift_field(
                (<MutableUnion>self)._fbthrift_get_field_value(field_id),
                field_id,
                self,
                transitive_annotation=transitive_annotation(),
            ),
        )
    else:
        return property(
            lambda self: (<MutableUnion>self)._fbthrift_get_field_value(field_id)
        )


class MutableUnionMeta(type):
    """Metaclass for all generated (mutable) thrift-python Union types."""

    def __new__(cls, union_name, bases, union_class_namespace):
        """
        Returns a new Thrift Union class with the given name and members.
        """
        if bases:
            raise TypeError(
                "Inheriting from thrift-python data types is forbidden: "
                f"'{union_name}' cannot inherit from '{bases[0].__name__}'"
            )

        field_infos = union_class_namespace.pop('_fbthrift_SPEC')
        num_fields = len(field_infos)

        union_class_namespace["_fbthrift_mutable_struct_info"] = MutableUnionInfo(
            union_name, field_infos
        )

        union_class_namespace["FbThriftUnionFieldEnum"] = enum.Enum(
            f"FbThriftUnionFieldEnum_{union_name}",
            _gen_mutable_union_field_enum_members(field_infos)
        )

        for field_info in field_infos:
            union_class_namespace[field_info.py_name] = _make_fget_union(
                field_info.id, field_info.adapter_info
            )

        return super().__new__(cls, union_name, (MutableUnion,), union_class_namespace)

    def _fbthrift_fill_spec(cls):
        (<MutableUnionInfo>cls._fbthrift_mutable_struct_info)._fill_mutable_union_info()
