#
# Autogenerated by Thrift
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#  @generated
#


import folly.iobuf as _fbthrift_iobuf

from thrift.py3.reflection cimport (
    NumberType as __NumberType,
    StructType as __StructType,
    Qualifier as __Qualifier,
)

cimport facebook.thrift.annotation.cpp.types as _facebook_thrift_annotation_cpp_types
cimport facebook.thrift.annotation.thrift.types as _facebook_thrift_annotation_thrift_types
cimport include.types as _include_types

cimport module.types as _module_types

from thrift.py3.types cimport (
    constant_shared_ptr,
    default_inst,
)


cdef __StructSpec get_reflection__decorated_struct():
    cdef _module_types.decorated_struct defaults = _module_types.decorated_struct._fbthrift_create(
        constant_shared_ptr[_module_types.cdecorated_struct](
            default_inst[_module_types.cdecorated_struct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="decorated_struct",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.declare_equal_to""": """1""",            """cpp.declare_hash""": """1""",        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="field",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__ContainerStruct():
    cdef _module_types.ContainerStruct defaults = _module_types.ContainerStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cContainerStruct](
            default_inst[_module_types.cContainerStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="ContainerStruct",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=12,
            name="fieldA",
            type=_module_types.List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="fieldB",
            type=_module_types.std_list__List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="fieldC",
            type=_module_types.std_deque__List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=4,
            name="fieldD",
            type=_module_types.folly_fbvector__List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=5,
            name="fieldE",
            type=_module_types.folly_small_vector__List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=6,
            name="fieldF",
            type=_module_types.folly_sorted_vector_set__Set__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=7,
            name="fieldG",
            type=_module_types.folly_sorted_vector_map__Map__i32_string,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=8,
            name="fieldH",
            type=_module_types.std_unordered_map__Map__i32_string,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__CppTypeStruct():
    cdef _module_types.CppTypeStruct defaults = _module_types.CppTypeStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cCppTypeStruct](
            default_inst[_module_types.cCppTypeStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="CppTypeStruct",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="fieldA",
            type=_module_types.std_list_int32_t__List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__VirtualStruct():
    cdef _module_types.VirtualStruct defaults = _module_types.VirtualStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cVirtualStruct](
            default_inst[_module_types.cVirtualStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="VirtualStruct",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.virtual""": """1""",        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="MyIntField",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__MyStructWithForwardRefEnum():
    cdef _module_types.MyStructWithForwardRefEnum defaults = _module_types.MyStructWithForwardRefEnum._fbthrift_create(
        constant_shared_ptr[_module_types.cMyStructWithForwardRefEnum](
            default_inst[_module_types.cMyStructWithForwardRefEnum]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="MyStructWithForwardRefEnum",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="a",
            type=_module_types.MyForwardRefEnum,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.a,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="b",
            type=_module_types.MyForwardRefEnum,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.b,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__TrivialNumeric():
    cdef _module_types.TrivialNumeric defaults = _module_types.TrivialNumeric._fbthrift_create(
        constant_shared_ptr[_module_types.cTrivialNumeric](
            default_inst[_module_types.cTrivialNumeric]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="TrivialNumeric",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="a",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="b",
            type=bool,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__TrivialNestedWithDefault():
    cdef _module_types.TrivialNestedWithDefault defaults = _module_types.TrivialNestedWithDefault._fbthrift_create(
        constant_shared_ptr[_module_types.cTrivialNestedWithDefault](
            default_inst[_module_types.cTrivialNestedWithDefault]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="TrivialNestedWithDefault",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="z",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.z,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="n",
            type=_module_types.TrivialNumeric,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.n,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__ComplexString():
    cdef _module_types.ComplexString defaults = _module_types.ComplexString._fbthrift_create(
        constant_shared_ptr[_module_types.cComplexString](
            default_inst[_module_types.cComplexString]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="ComplexString",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="a",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="b",
            type=_module_types.Map__string_i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__ComplexNestedWithDefault():
    cdef _module_types.ComplexNestedWithDefault defaults = _module_types.ComplexNestedWithDefault._fbthrift_create(
        constant_shared_ptr[_module_types.cComplexNestedWithDefault](
            default_inst[_module_types.cComplexNestedWithDefault]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="ComplexNestedWithDefault",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="z",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.z,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="n",
            type=_module_types.ComplexString,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.n,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__MinPadding():
    cdef _module_types.MinPadding defaults = _module_types.MinPadding._fbthrift_create(
        constant_shared_ptr[_module_types.cMinPadding](
            default_inst[_module_types.cMinPadding]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="MinPadding",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="small",
            type=int,
            kind=__NumberType.BYTE,
            qualifier=__Qualifier.REQUIRED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="big",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.REQUIRED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="medium",
            type=int,
            kind=__NumberType.I16,
            qualifier=__Qualifier.REQUIRED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=4,
            name="biggish",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.REQUIRED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=5,
            name="tiny",
            type=int,
            kind=__NumberType.BYTE,
            qualifier=__Qualifier.REQUIRED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__MinPaddingWithCustomType():
    cdef _module_types.MinPaddingWithCustomType defaults = _module_types.MinPaddingWithCustomType._fbthrift_create(
        constant_shared_ptr[_module_types.cMinPaddingWithCustomType](
            default_inst[_module_types.cMinPaddingWithCustomType]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="MinPaddingWithCustomType",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="small",
            type=int,
            kind=__NumberType.BYTE,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="big",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="medium",
            type=int,
            kind=__NumberType.I16,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=4,
            name="biggish",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=5,
            name="tiny",
            type=int,
            kind=__NumberType.BYTE,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__MyStruct():
    cdef _module_types.MyStruct defaults = _module_types.MyStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cMyStruct](
            default_inst[_module_types.cMyStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="MyStruct",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.noncomparable""": """1""",        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="MyIntField",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="MyStringField",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="majorVer",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=4,
            name="data",
            type=_module_types.MyDataItem,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__MyDataItem():
    cdef _module_types.MyDataItem defaults = _module_types.MyDataItem._fbthrift_create(
        constant_shared_ptr[_module_types.cMyDataItem](
            default_inst[_module_types.cMyDataItem]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="MyDataItem",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.noncomparable""": """1""",        },
    )
    return spec
cdef __StructSpec get_reflection__Renaming():
    cdef _module_types.Renaming defaults = _module_types.Renaming._fbthrift_create(
        constant_shared_ptr[_module_types.cRenaming](
            default_inst[_module_types.cRenaming]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="Renaming",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.name""": """Renamed""",        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="foo",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
                """cpp.name""": """bar""",            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__AnnotatedTypes():
    cdef _module_types.AnnotatedTypes defaults = _module_types.AnnotatedTypes._fbthrift_create(
        constant_shared_ptr[_module_types.cAnnotatedTypes](
            default_inst[_module_types.cAnnotatedTypes]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="AnnotatedTypes",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="binary_field",
            type=bytes,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="list_field",
            type=_module_types.List__std_unordered_map__Map__i32_string,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__ForwardUsageRoot():
    cdef _module_types.ForwardUsageRoot defaults = _module_types.ForwardUsageRoot._fbthrift_create(
        constant_shared_ptr[_module_types.cForwardUsageRoot](
            default_inst[_module_types.cForwardUsageRoot]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="ForwardUsageRoot",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="ForwardUsageStruct",
            type=_module_types.ForwardUsageStruct,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.OPTIONAL,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="ForwardUsageByRef",
            type=_module_types.ForwardUsageByRef,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.OPTIONAL,
            default=None,
            annotations={
                """cpp.ref""": """true""",            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__ForwardUsageStruct():
    cdef _module_types.ForwardUsageStruct defaults = _module_types.ForwardUsageStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cForwardUsageStruct](
            default_inst[_module_types.cForwardUsageStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="ForwardUsageStruct",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="foo",
            type=_module_types.ForwardUsageRoot,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.OPTIONAL,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__ForwardUsageByRef():
    cdef _module_types.ForwardUsageByRef defaults = _module_types.ForwardUsageByRef._fbthrift_create(
        constant_shared_ptr[_module_types.cForwardUsageByRef](
            default_inst[_module_types.cForwardUsageByRef]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="ForwardUsageByRef",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="foo",
            type=_module_types.ForwardUsageRoot,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.OPTIONAL,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__NoexceptMoveEmpty():
    cdef _module_types.NoexceptMoveEmpty defaults = _module_types.NoexceptMoveEmpty._fbthrift_create(
        constant_shared_ptr[_module_types.cNoexceptMoveEmpty](
            default_inst[_module_types.cNoexceptMoveEmpty]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="NoexceptMoveEmpty",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    return spec
cdef __StructSpec get_reflection__NoexceptMoveSimpleStruct():
    cdef _module_types.NoexceptMoveSimpleStruct defaults = _module_types.NoexceptMoveSimpleStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cNoexceptMoveSimpleStruct](
            default_inst[_module_types.cNoexceptMoveSimpleStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="NoexceptMoveSimpleStruct",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="boolField",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__NoexceptMoveComplexStruct():
    cdef _module_types.NoexceptMoveComplexStruct defaults = _module_types.NoexceptMoveComplexStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cNoexceptMoveComplexStruct](
            default_inst[_module_types.cNoexceptMoveComplexStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="NoexceptMoveComplexStruct",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="MyBoolField",
            type=bool,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="MyIntField",
            type=int,
            kind=__NumberType.I64,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.MyIntField,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="MyStringField",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.MyStringField,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=4,
            name="MyStringField2",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=5,
            name="MyBinaryField",
            type=bytes,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=6,
            name="MyBinaryField2",
            type=bytes,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.OPTIONAL,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=7,
            name="MyBinaryField3",
            type=bytes,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.REQUIRED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=8,
            name="MyBinaryListField4",
            type=_module_types.List__binary,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=9,
            name="MyMapEnumAndInt",
            type=_module_types.Map__MyEnumA_string,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=defaults.MyMapEnumAndInt,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__NoExceptMoveUnion():
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="NoExceptMoveUnion",
        kind=__StructType.UNION,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="string_field",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="i32_field",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__AllocatorAware():
    cdef _module_types.AllocatorAware defaults = _module_types.AllocatorAware._fbthrift_create(
        constant_shared_ptr[_module_types.cAllocatorAware](
            default_inst[_module_types.cAllocatorAware]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="AllocatorAware",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.allocator""": """some_allocator""",        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="aa_list",
            type=_module_types.List__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="aa_set",
            type=_module_types.Set__i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="aa_map",
            type=_module_types.Map__i32_i32,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=4,
            name="aa_string",
            type=str,
            kind=__NumberType.NOT_A_NUMBER,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=5,
            name="not_a_container",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=6,
            name="aa_unique",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
                """cpp.ref_type""": """unique""",            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=7,
            name="aa_shared",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
                """cpp.ref_type""": """shared""",            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__AllocatorAware2():
    cdef _module_types.AllocatorAware2 defaults = _module_types.AllocatorAware2._fbthrift_create(
        constant_shared_ptr[_module_types.cAllocatorAware2](
            default_inst[_module_types.cAllocatorAware2]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="AllocatorAware2",
        kind=__StructType.STRUCT,
        annotations={
            """cpp.allocator""": """some_allocator""",        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="not_a_container",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__TypedefStruct():
    cdef _module_types.TypedefStruct defaults = _module_types.TypedefStruct._fbthrift_create(
        constant_shared_ptr[_module_types.cTypedefStruct](
            default_inst[_module_types.cTypedefStruct]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="TypedefStruct",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="i32_field",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=2,
            name="IntTypedef_field",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=3,
            name="UintTypedef_field",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __StructSpec get_reflection__StructWithDoubleUnderscores():
    cdef _module_types.StructWithDoubleUnderscores defaults = _module_types.StructWithDoubleUnderscores._fbthrift_create(
        constant_shared_ptr[_module_types.cStructWithDoubleUnderscores](
            default_inst[_module_types.cStructWithDoubleUnderscores]()
        )
    )
    cdef __StructSpec spec = __StructSpec._fbthrift_create(
        name="StructWithDoubleUnderscores",
        kind=__StructType.STRUCT,
        annotations={
        },
    )
    spec.add_field(
        __FieldSpec._fbthrift_create(
            id=1,
            name="__field",
            type=int,
            kind=__NumberType.I32,
            qualifier=__Qualifier.UNQUALIFIED,
            default=None,
            annotations={
            },
        ),
    )
    return spec
cdef __MapSpec get_reflection__std_unordered_map__Map__i32_string():
    return __MapSpec._fbthrift_create(
        key=int,
        key_kind=__NumberType.I32,
        value=str,
        value_kind=__NumberType.NOT_A_NUMBER,
    )

cdef __ListSpec get_reflection__List__i64():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I64,
    )

cdef __MapSpec get_reflection__Map__binary_i64():
    return __MapSpec._fbthrift_create(
        key=bytes,
        key_kind=__NumberType.NOT_A_NUMBER,
        value=int,
        value_kind=__NumberType.I64,
    )

cdef __ListSpec get_reflection__List__i32():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
    )

cdef __ListSpec get_reflection__std_list__List__i32():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
    )

cdef __ListSpec get_reflection__std_deque__List__i32():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
    )

cdef __ListSpec get_reflection__folly_fbvector__List__i32():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
    )

cdef __ListSpec get_reflection__folly_small_vector__List__i32():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
    )

cdef __SetSpec get_reflection__folly_sorted_vector_set__Set__i32():
    return __SetSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
     )

cdef __MapSpec get_reflection__folly_sorted_vector_map__Map__i32_string():
    return __MapSpec._fbthrift_create(
        key=int,
        key_kind=__NumberType.I32,
        value=str,
        value_kind=__NumberType.NOT_A_NUMBER,
    )

cdef __ListSpec get_reflection__std_list_int32_t__List__i32():
    return __ListSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
    )

cdef __MapSpec get_reflection__Map__string_i32():
    return __MapSpec._fbthrift_create(
        key=str,
        key_kind=__NumberType.NOT_A_NUMBER,
        value=int,
        value_kind=__NumberType.I32,
    )

cdef __ListSpec get_reflection__List__std_unordered_map__Map__i32_string():
    return __ListSpec._fbthrift_create(
        value=_module_types.std_unordered_map__Map__i32_string,
        kind=__NumberType.NOT_A_NUMBER,
    )

cdef __ListSpec get_reflection__List__binary():
    return __ListSpec._fbthrift_create(
        value=bytes,
        kind=__NumberType.NOT_A_NUMBER,
    )

cdef __MapSpec get_reflection__Map__MyEnumA_string():
    return __MapSpec._fbthrift_create(
        key=_module_types.MyEnumA,
        key_kind=__NumberType.NOT_A_NUMBER,
        value=str,
        value_kind=__NumberType.NOT_A_NUMBER,
    )

cdef __SetSpec get_reflection__Set__i32():
    return __SetSpec._fbthrift_create(
        value=int,
        kind=__NumberType.I32,
     )

cdef __MapSpec get_reflection__Map__i32_i32():
    return __MapSpec._fbthrift_create(
        key=int,
        key_kind=__NumberType.I32,
        value=int,
        value_kind=__NumberType.I32,
    )

