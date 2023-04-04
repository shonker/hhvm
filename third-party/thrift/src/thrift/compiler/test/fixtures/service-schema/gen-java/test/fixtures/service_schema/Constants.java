/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

package test.fixtures.service_schema;

import com.facebook.swift.codec.*;
import com.google.common.collect.*;
import java.util.*;

@SwiftGenerated
public final class Constants {
    private Constants() {}

    public static final com.facebook.thrift.type.Schema SCHEMA_PRIMITIVES_SERVICE = new com.facebook.thrift.type.Schema.Builder().setDefinitions(ImmutableList.<com.facebook.thrift.type.Definition>builder()
        .add(com.facebook.thrift.type.Definition.fromEnumDef(new com.facebook.thrift.type.Enum.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("Result").build()).setValues(ImmutableList.<com.facebook.thrift.type.EnumValue>builder()
        .add(new com.facebook.thrift.type.EnumValue.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("OK").build()).setValue(0).build())
        .add(new com.facebook.thrift.type.EnumValue.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("SO_SO").build()).setValue(1).build())
        .add(new com.facebook.thrift.type.EnumValue.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("GOOD").build()).setValue(2).build())
        .build()).build()))
        .add(com.facebook.thrift.type.Definition.fromExceptionDef(new com.facebook.thrift.type.Exception.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("CustomException").build()).setFields(ImmutableList.<com.facebook.thrift.type.Field>builder()
        .add(new com.facebook.thrift.type.Field.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("name").build()).setId((short)1).setQualifier(com.facebook.thrift.type.FieldQualifier.FILL).setType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromStringType(com.facebook.thrift.standard_type.Void.UNUSED)).build()).build())
        .build()).setSafety(com.facebook.thrift.type.ErrorSafety.UNSPECIFIED).setKind(com.facebook.thrift.type.ErrorKind.UNSPECIFIED).setBlame(com.facebook.thrift.type.ErrorBlame.UNSPECIFIED).build()))
        .add(com.facebook.thrift.type.Definition.fromServiceDef(new com.facebook.thrift.type.Service.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("PrimitivesService").setAnnotations(ImmutableSet.<Long>builder()
        .add(1L)
        .build()).build()).setFunctions(ImmutableList.<com.facebook.thrift.type.Function>builder()
        .add(new com.facebook.thrift.type.Function.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("init").build()).setQualifier(com.facebook.thrift.type.FunctionQualifier.UNSPECIFIED).setReturnTypes(ImmutableList.<com.facebook.thrift.type.ReturnType>builder()
        .add(com.facebook.thrift.type.ReturnType.fromThriftType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromI64Type(com.facebook.thrift.standard_type.Void.UNUSED)).build()))
        .build()).setParamlist(new com.facebook.thrift.type.Paramlist.Builder().setFields(ImmutableList.<com.facebook.thrift.type.Field>builder()
        .add(new com.facebook.thrift.type.Field.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("param0").build()).setId((short)1).setQualifier(com.facebook.thrift.type.FieldQualifier.FILL).setType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromI64Type(com.facebook.thrift.standard_type.Void.UNUSED)).build()).build())
        .add(new com.facebook.thrift.type.Field.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("param1").build()).setId((short)2).setQualifier(com.facebook.thrift.type.FieldQualifier.FILL).setType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromI64Type(com.facebook.thrift.standard_type.Void.UNUSED)).build()).build())
        .build()).build()).build())
        .add(new com.facebook.thrift.type.Function.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("method_that_throws").build()).setQualifier(com.facebook.thrift.type.FunctionQualifier.UNSPECIFIED).setReturnTypes(ImmutableList.<com.facebook.thrift.type.ReturnType>builder()
        .add(com.facebook.thrift.type.ReturnType.fromThriftType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromEnumType(com.facebook.thrift.standard_type.TypeUri.fromUri(""))).build()))
        .build()).setParamlist(new com.facebook.thrift.type.Paramlist.Builder().setFields(ImmutableList.<com.facebook.thrift.type.Field>builder()
        .build()).build()).setExceptions(ImmutableList.<com.facebook.thrift.type.Field>builder()
        .add(new com.facebook.thrift.type.Field.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("e").build()).setId((short)1).setQualifier(com.facebook.thrift.type.FieldQualifier.FILL).setType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromExceptionType(com.facebook.thrift.standard_type.TypeUri.fromUri(""))).build()).build())
        .build()).build())
        .add(new com.facebook.thrift.type.Function.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("return_void_method").build()).setQualifier(com.facebook.thrift.type.FunctionQualifier.UNSPECIFIED).setReturnTypes(ImmutableList.<com.facebook.thrift.type.ReturnType>builder()
        .add(com.facebook.thrift.type.ReturnType.fromThriftType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName).build()))
        .build()).setParamlist(new com.facebook.thrift.type.Paramlist.Builder().setFields(ImmutableList.<com.facebook.thrift.type.Field>builder()
        .add(new com.facebook.thrift.type.Field.Builder().setAttrs(new com.facebook.thrift.type.DefinitionAttrs.Builder().setName("id").build()).setId((short)1).setQualifier(com.facebook.thrift.type.FieldQualifier.FILL).setType(new com.facebook.thrift.type_swift.TypeStruct.Builder().setName(com.facebook.thrift.standard_type.TypeName.fromI64Type(com.facebook.thrift.standard_type.Void.UNUSED)).build()).build())
        .build()).build()).build())
        .build()).build()))
        .build()).build();
}
