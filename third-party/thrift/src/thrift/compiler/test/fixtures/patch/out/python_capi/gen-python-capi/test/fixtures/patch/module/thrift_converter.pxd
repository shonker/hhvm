
#
# Autogenerated by Thrift
#
# DO NOT EDIT
#  @generated
#


cdef extern from "thrift/compiler/test/fixtures/patch/gen-cpp2/module_types.h":
    cdef cppclass cMyData "::test::fixtures::patch::MyData":
        cMyData()
    cdef cppclass cMyDataWithCustomDefault "::test::fixtures::patch::MyDataWithCustomDefault":
        cMyDataWithCustomDefault()
    cdef cppclass cInnerUnion "::test::fixtures::patch::InnerUnion":
        cInnerUnion()
    cdef cppclass cMyUnion "::test::fixtures::patch::MyUnion":
        cMyUnion()
    cdef cppclass cMyStruct "::test::fixtures::patch::MyStruct":
        cMyStruct()
    cdef cppclass cLateDefStruct "::test::fixtures::patch::LateDefStruct":
        cLateDefStruct()
    cdef cppclass cRecursive "::test::fixtures::patch::Recursive":
        cRecursive()
    cdef cppclass cBar "::test::fixtures::patch::Bar":
        cBar()
    cdef cppclass cLoop "::test::fixtures::patch::Loop":
        cLoop()
    cdef cppclass cRefFields "::test::fixtures::patch::RefFields":
        cRefFields()
    cdef cppclass cMyDataPatchStructInternalDoNotUse "::test::fixtures::patch::MyDataPatchStruct":
        cMyDataPatchStructInternalDoNotUse()
    cdef cppclass cMyDataFieldPatch "::test::fixtures::patch::MyDataFieldPatchStruct":
        cMyDataFieldPatch()
    cdef cppclass cMyDataEnsureStruct "::test::fixtures::patch::MyDataEnsureStruct":
        cMyDataEnsureStruct()
    cdef cppclass cMyDataSafePatch "::test::fixtures::patch::MyDataSafePatch":
        cMyDataSafePatch()
    cdef cppclass cMyDataWithCustomDefaultPatchStructInternalDoNotUse "::test::fixtures::patch::MyDataWithCustomDefaultPatchStruct":
        cMyDataWithCustomDefaultPatchStructInternalDoNotUse()
    cdef cppclass cMyDataWithCustomDefaultFieldPatch "::test::fixtures::patch::MyDataWithCustomDefaultFieldPatchStruct":
        cMyDataWithCustomDefaultFieldPatch()
    cdef cppclass cMyDataWithCustomDefaultEnsureStruct "::test::fixtures::patch::MyDataWithCustomDefaultEnsureStruct":
        cMyDataWithCustomDefaultEnsureStruct()
    cdef cppclass cMyDataWithCustomDefaultSafePatch "::test::fixtures::patch::MyDataWithCustomDefaultSafePatch":
        cMyDataWithCustomDefaultSafePatch()
    cdef cppclass cInnerUnionPatch "::test::fixtures::patch::InnerUnionPatchStruct":
        cInnerUnionPatch()
    cdef cppclass cInnerUnionFieldPatch "::test::fixtures::patch::InnerUnionFieldPatchStruct":
        cInnerUnionFieldPatch()
    cdef cppclass cInnerUnionSafePatch "::test::fixtures::patch::InnerUnionSafePatch":
        cInnerUnionSafePatch()
    cdef cppclass cMyUnionPatch "::test::fixtures::patch::MyUnionPatchStruct":
        cMyUnionPatch()
    cdef cppclass cMyUnionFieldPatch "::test::fixtures::patch::MyUnionFieldPatchStruct":
        cMyUnionFieldPatch()
    cdef cppclass cMyUnionSafePatch "::test::fixtures::patch::MyUnionSafePatch":
        cMyUnionSafePatch()
    cdef cppclass cMyStructPatchStructInternalDoNotUse "::test::fixtures::patch::MyStructPatchStruct":
        cMyStructPatchStructInternalDoNotUse()
    cdef cppclass cMyStructField10Patch "::test::fixtures::patch::MyStructField10PatchStruct":
        cMyStructField10Patch()
    cdef cppclass cMyStructField23Patch "::test::fixtures::patch::MyStructField23PatchStruct":
        cMyStructField23Patch()
    cdef cppclass cMyStructField26Patch "::test::fixtures::patch::MyStructField26PatchStruct":
        cMyStructField26Patch()
    cdef cppclass cMyStructField27Patch "::test::fixtures::patch::MyStructField27PatchStruct":
        cMyStructField27Patch()
    cdef cppclass cMyStructField28Patch "::test::fixtures::patch::MyStructField28PatchStruct":
        cMyStructField28Patch()
    cdef cppclass cMyStructField29Patch "::test::fixtures::patch::MyStructField29PatchStruct":
        cMyStructField29Patch()
    cdef cppclass cMyStructField30Patch "::test::fixtures::patch::MyStructField30PatchStruct":
        cMyStructField30Patch()
    cdef cppclass cMyStructField30Patch1 "::test::fixtures::patch::MyStructField30Patch1Struct":
        cMyStructField30Patch1()
    cdef cppclass cMyStructFieldPatch "::test::fixtures::patch::MyStructFieldPatchStruct":
        cMyStructFieldPatch()
    cdef cppclass cMyStructEnsureStruct "::test::fixtures::patch::MyStructEnsureStruct":
        cMyStructEnsureStruct()
    cdef cppclass cMyStructSafePatch "::test::fixtures::patch::MyStructSafePatch":
        cMyStructSafePatch()
    cdef cppclass cLateDefStructPatchStructInternalDoNotUse "::test::fixtures::patch::LateDefStructPatchStruct":
        cLateDefStructPatchStructInternalDoNotUse()
    cdef cppclass cLateDefStructFieldPatch "::test::fixtures::patch::LateDefStructFieldPatchStruct":
        cLateDefStructFieldPatch()
    cdef cppclass cLateDefStructEnsureStruct "::test::fixtures::patch::LateDefStructEnsureStruct":
        cLateDefStructEnsureStruct()
    cdef cppclass cLateDefStructSafePatch "::test::fixtures::patch::LateDefStructSafePatch":
        cLateDefStructSafePatch()
    cdef cppclass cRecursivePatchStructInternalDoNotUse "::test::fixtures::patch::RecursivePatchStruct":
        cRecursivePatchStructInternalDoNotUse()
    cdef cppclass cRecursiveField1Patch "::test::fixtures::patch::RecursiveField1PatchStruct":
        cRecursiveField1Patch()
    cdef cppclass cRecursiveFieldPatch "::test::fixtures::patch::RecursiveFieldPatchStruct":
        cRecursiveFieldPatch()
    cdef cppclass cRecursiveEnsureStruct "::test::fixtures::patch::RecursiveEnsureStruct":
        cRecursiveEnsureStruct()
    cdef cppclass cRecursiveSafePatch "::test::fixtures::patch::RecursiveSafePatch":
        cRecursiveSafePatch()
    cdef cppclass cBarPatchStructInternalDoNotUse "::test::fixtures::patch::BarPatchStruct":
        cBarPatchStructInternalDoNotUse()
    cdef cppclass cBarFieldPatch "::test::fixtures::patch::BarFieldPatchStruct":
        cBarFieldPatch()
    cdef cppclass cBarEnsureStruct "::test::fixtures::patch::BarEnsureStruct":
        cBarEnsureStruct()
    cdef cppclass cBarSafePatch "::test::fixtures::patch::BarSafePatch":
        cBarSafePatch()
    cdef cppclass cLoopPatchStructInternalDoNotUse "::test::fixtures::patch::LoopPatchStruct":
        cLoopPatchStructInternalDoNotUse()
    cdef cppclass cLoopSafePatch "::test::fixtures::patch::LoopSafePatch":
        cLoopSafePatch()
    cdef cppclass cRefFieldsPatchStructInternalDoNotUse "::test::fixtures::patch::RefFieldsPatchStruct":
        cRefFieldsPatchStructInternalDoNotUse()
    cdef cppclass cRefFieldsField1Patch "::test::fixtures::patch::RefFieldsField1PatchStruct":
        cRefFieldsField1Patch()
    cdef cppclass cRefFieldsField4Patch "::test::fixtures::patch::RefFieldsField4PatchStruct":
        cRefFieldsField4Patch()
    cdef cppclass cRefFieldsField7Patch "::test::fixtures::patch::RefFieldsField7PatchStruct":
        cRefFieldsField7Patch()
    cdef cppclass cRefFieldsFieldPatch "::test::fixtures::patch::RefFieldsFieldPatchStruct":
        cRefFieldsFieldPatch()
    cdef cppclass cRefFieldsEnsureStruct "::test::fixtures::patch::RefFieldsEnsureStruct":
        cRefFieldsEnsureStruct()
    cdef cppclass cRefFieldsSafePatch "::test::fixtures::patch::RefFieldsSafePatch":
        cRefFieldsSafePatch()

cdef extern from "thrift/compiler/test/fixtures/patch/gen-cpp2/module_types.h":
    cdef cppclass cMyEnum "::test::fixtures::patch::MyEnum":
        pass
cdef cMyData MyData_convert_to_cpp(object inst) except*
cdef object MyData_from_cpp(const cMyData& c_struct)

cdef cMyDataWithCustomDefault MyDataWithCustomDefault_convert_to_cpp(object inst) except*
cdef object MyDataWithCustomDefault_from_cpp(const cMyDataWithCustomDefault& c_struct)

cdef cInnerUnion InnerUnion_convert_to_cpp(object inst) except*
cdef object InnerUnion_from_cpp(const cInnerUnion& c_struct)

cdef cMyUnion MyUnion_convert_to_cpp(object inst) except*
cdef object MyUnion_from_cpp(const cMyUnion& c_struct)

cdef cMyStruct MyStruct_convert_to_cpp(object inst) except*
cdef object MyStruct_from_cpp(const cMyStruct& c_struct)

cdef cLateDefStruct LateDefStruct_convert_to_cpp(object inst) except*
cdef object LateDefStruct_from_cpp(const cLateDefStruct& c_struct)

cdef cRecursive Recursive_convert_to_cpp(object inst) except*
cdef object Recursive_from_cpp(const cRecursive& c_struct)

cdef cBar Bar_convert_to_cpp(object inst) except*
cdef object Bar_from_cpp(const cBar& c_struct)

cdef cLoop Loop_convert_to_cpp(object inst) except*
cdef object Loop_from_cpp(const cLoop& c_struct)

cdef cRefFields RefFields_convert_to_cpp(object inst) except*
cdef object RefFields_from_cpp(const cRefFields& c_struct)

cdef cMyDataPatchStructInternalDoNotUse MyDataPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object MyDataPatchStructInternalDoNotUse_from_cpp(const cMyDataPatchStructInternalDoNotUse& c_struct)

cdef cMyDataFieldPatch MyDataFieldPatch_convert_to_cpp(object inst) except*
cdef object MyDataFieldPatch_from_cpp(const cMyDataFieldPatch& c_struct)

cdef cMyDataEnsureStruct MyDataEnsureStruct_convert_to_cpp(object inst) except*
cdef object MyDataEnsureStruct_from_cpp(const cMyDataEnsureStruct& c_struct)

cdef cMyDataSafePatch MyDataSafePatch_convert_to_cpp(object inst) except*
cdef object MyDataSafePatch_from_cpp(const cMyDataSafePatch& c_struct)

cdef cMyDataWithCustomDefaultPatchStructInternalDoNotUse MyDataWithCustomDefaultPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object MyDataWithCustomDefaultPatchStructInternalDoNotUse_from_cpp(const cMyDataWithCustomDefaultPatchStructInternalDoNotUse& c_struct)

cdef cMyDataWithCustomDefaultFieldPatch MyDataWithCustomDefaultFieldPatch_convert_to_cpp(object inst) except*
cdef object MyDataWithCustomDefaultFieldPatch_from_cpp(const cMyDataWithCustomDefaultFieldPatch& c_struct)

cdef cMyDataWithCustomDefaultEnsureStruct MyDataWithCustomDefaultEnsureStruct_convert_to_cpp(object inst) except*
cdef object MyDataWithCustomDefaultEnsureStruct_from_cpp(const cMyDataWithCustomDefaultEnsureStruct& c_struct)

cdef cMyDataWithCustomDefaultSafePatch MyDataWithCustomDefaultSafePatch_convert_to_cpp(object inst) except*
cdef object MyDataWithCustomDefaultSafePatch_from_cpp(const cMyDataWithCustomDefaultSafePatch& c_struct)

cdef cInnerUnionPatch InnerUnionPatch_convert_to_cpp(object inst) except*
cdef object InnerUnionPatch_from_cpp(const cInnerUnionPatch& c_struct)

cdef cInnerUnionFieldPatch InnerUnionFieldPatch_convert_to_cpp(object inst) except*
cdef object InnerUnionFieldPatch_from_cpp(const cInnerUnionFieldPatch& c_struct)

cdef cInnerUnionSafePatch InnerUnionSafePatch_convert_to_cpp(object inst) except*
cdef object InnerUnionSafePatch_from_cpp(const cInnerUnionSafePatch& c_struct)

cdef cMyUnionPatch MyUnionPatch_convert_to_cpp(object inst) except*
cdef object MyUnionPatch_from_cpp(const cMyUnionPatch& c_struct)

cdef cMyUnionFieldPatch MyUnionFieldPatch_convert_to_cpp(object inst) except*
cdef object MyUnionFieldPatch_from_cpp(const cMyUnionFieldPatch& c_struct)

cdef cMyUnionSafePatch MyUnionSafePatch_convert_to_cpp(object inst) except*
cdef object MyUnionSafePatch_from_cpp(const cMyUnionSafePatch& c_struct)

cdef cMyStructPatchStructInternalDoNotUse MyStructPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object MyStructPatchStructInternalDoNotUse_from_cpp(const cMyStructPatchStructInternalDoNotUse& c_struct)

cdef cMyStructField10Patch MyStructField10Patch_convert_to_cpp(object inst) except*
cdef object MyStructField10Patch_from_cpp(const cMyStructField10Patch& c_struct)

cdef cMyStructField23Patch MyStructField23Patch_convert_to_cpp(object inst) except*
cdef object MyStructField23Patch_from_cpp(const cMyStructField23Patch& c_struct)

cdef cMyStructField26Patch MyStructField26Patch_convert_to_cpp(object inst) except*
cdef object MyStructField26Patch_from_cpp(const cMyStructField26Patch& c_struct)

cdef cMyStructField27Patch MyStructField27Patch_convert_to_cpp(object inst) except*
cdef object MyStructField27Patch_from_cpp(const cMyStructField27Patch& c_struct)

cdef cMyStructField28Patch MyStructField28Patch_convert_to_cpp(object inst) except*
cdef object MyStructField28Patch_from_cpp(const cMyStructField28Patch& c_struct)

cdef cMyStructField29Patch MyStructField29Patch_convert_to_cpp(object inst) except*
cdef object MyStructField29Patch_from_cpp(const cMyStructField29Patch& c_struct)

cdef cMyStructField30Patch MyStructField30Patch_convert_to_cpp(object inst) except*
cdef object MyStructField30Patch_from_cpp(const cMyStructField30Patch& c_struct)

cdef cMyStructField30Patch1 MyStructField30Patch1_convert_to_cpp(object inst) except*
cdef object MyStructField30Patch1_from_cpp(const cMyStructField30Patch1& c_struct)

cdef cMyStructFieldPatch MyStructFieldPatch_convert_to_cpp(object inst) except*
cdef object MyStructFieldPatch_from_cpp(const cMyStructFieldPatch& c_struct)

cdef cMyStructEnsureStruct MyStructEnsureStruct_convert_to_cpp(object inst) except*
cdef object MyStructEnsureStruct_from_cpp(const cMyStructEnsureStruct& c_struct)

cdef cMyStructSafePatch MyStructSafePatch_convert_to_cpp(object inst) except*
cdef object MyStructSafePatch_from_cpp(const cMyStructSafePatch& c_struct)

cdef cLateDefStructPatchStructInternalDoNotUse LateDefStructPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object LateDefStructPatchStructInternalDoNotUse_from_cpp(const cLateDefStructPatchStructInternalDoNotUse& c_struct)

cdef cLateDefStructFieldPatch LateDefStructFieldPatch_convert_to_cpp(object inst) except*
cdef object LateDefStructFieldPatch_from_cpp(const cLateDefStructFieldPatch& c_struct)

cdef cLateDefStructEnsureStruct LateDefStructEnsureStruct_convert_to_cpp(object inst) except*
cdef object LateDefStructEnsureStruct_from_cpp(const cLateDefStructEnsureStruct& c_struct)

cdef cLateDefStructSafePatch LateDefStructSafePatch_convert_to_cpp(object inst) except*
cdef object LateDefStructSafePatch_from_cpp(const cLateDefStructSafePatch& c_struct)

cdef cRecursivePatchStructInternalDoNotUse RecursivePatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object RecursivePatchStructInternalDoNotUse_from_cpp(const cRecursivePatchStructInternalDoNotUse& c_struct)

cdef cRecursiveField1Patch RecursiveField1Patch_convert_to_cpp(object inst) except*
cdef object RecursiveField1Patch_from_cpp(const cRecursiveField1Patch& c_struct)

cdef cRecursiveFieldPatch RecursiveFieldPatch_convert_to_cpp(object inst) except*
cdef object RecursiveFieldPatch_from_cpp(const cRecursiveFieldPatch& c_struct)

cdef cRecursiveEnsureStruct RecursiveEnsureStruct_convert_to_cpp(object inst) except*
cdef object RecursiveEnsureStruct_from_cpp(const cRecursiveEnsureStruct& c_struct)

cdef cRecursiveSafePatch RecursiveSafePatch_convert_to_cpp(object inst) except*
cdef object RecursiveSafePatch_from_cpp(const cRecursiveSafePatch& c_struct)

cdef cBarPatchStructInternalDoNotUse BarPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object BarPatchStructInternalDoNotUse_from_cpp(const cBarPatchStructInternalDoNotUse& c_struct)

cdef cBarFieldPatch BarFieldPatch_convert_to_cpp(object inst) except*
cdef object BarFieldPatch_from_cpp(const cBarFieldPatch& c_struct)

cdef cBarEnsureStruct BarEnsureStruct_convert_to_cpp(object inst) except*
cdef object BarEnsureStruct_from_cpp(const cBarEnsureStruct& c_struct)

cdef cBarSafePatch BarSafePatch_convert_to_cpp(object inst) except*
cdef object BarSafePatch_from_cpp(const cBarSafePatch& c_struct)

cdef cLoopPatchStructInternalDoNotUse LoopPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object LoopPatchStructInternalDoNotUse_from_cpp(const cLoopPatchStructInternalDoNotUse& c_struct)

cdef cLoopSafePatch LoopSafePatch_convert_to_cpp(object inst) except*
cdef object LoopSafePatch_from_cpp(const cLoopSafePatch& c_struct)

cdef cRefFieldsPatchStructInternalDoNotUse RefFieldsPatchStructInternalDoNotUse_convert_to_cpp(object inst) except*
cdef object RefFieldsPatchStructInternalDoNotUse_from_cpp(const cRefFieldsPatchStructInternalDoNotUse& c_struct)

cdef cRefFieldsField1Patch RefFieldsField1Patch_convert_to_cpp(object inst) except*
cdef object RefFieldsField1Patch_from_cpp(const cRefFieldsField1Patch& c_struct)

cdef cRefFieldsField4Patch RefFieldsField4Patch_convert_to_cpp(object inst) except*
cdef object RefFieldsField4Patch_from_cpp(const cRefFieldsField4Patch& c_struct)

cdef cRefFieldsField7Patch RefFieldsField7Patch_convert_to_cpp(object inst) except*
cdef object RefFieldsField7Patch_from_cpp(const cRefFieldsField7Patch& c_struct)

cdef cRefFieldsFieldPatch RefFieldsFieldPatch_convert_to_cpp(object inst) except*
cdef object RefFieldsFieldPatch_from_cpp(const cRefFieldsFieldPatch& c_struct)

cdef cRefFieldsEnsureStruct RefFieldsEnsureStruct_convert_to_cpp(object inst) except*
cdef object RefFieldsEnsureStruct_from_cpp(const cRefFieldsEnsureStruct& c_struct)

cdef cRefFieldsSafePatch RefFieldsSafePatch_convert_to_cpp(object inst) except*
cdef object RefFieldsSafePatch_from_cpp(const cRefFieldsSafePatch& c_struct)

