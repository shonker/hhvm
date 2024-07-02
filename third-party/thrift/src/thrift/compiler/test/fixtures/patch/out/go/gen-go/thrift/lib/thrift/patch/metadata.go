// Autogenerated by Thrift for thrift/lib/thrift/patch.thrift
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//  @generated

package patch

import (
    standard "thrift/lib/thrift/standard"
    id "thrift/lib/thrift/id"
    thrift "github.com/facebook/fbthrift/thrift/lib/go/thrift"
    metadata "github.com/facebook/fbthrift/thrift/lib/thrift/metadata"
)

// mapsCopy is a copy of maps.Copy from Go 1.21
// TODO: remove mapsCopy once we can safely upgrade to Go 1.21 without requiring any rollback.
func mapsCopy[M1 ~map[K]V, M2 ~map[K]V, K comparable, V any](dst M1, src M2) {
	for k, v := range src {
		dst[k] = v
	}
}

var _ = standard.GoUnusedProtection__
var _ = id.GoUnusedProtection__
// (needed to ensure safety because of naive import list construction)
var _ = thrift.ZERO
// TODO: uncomment when can safely upgrade to Go 1.21 without requiring any rollback.
// var _ = maps.Copy[map[int]int, map[int]int]
var _ = metadata.GoUnusedProtection__

// Premade Thrift types
var (
    premadeThriftType_bool = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_BOOL_TYPE.Ptr(),
            )
    premadeThriftType_byte = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_BYTE_TYPE.Ptr(),
            )
    premadeThriftType_i16 = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_I16_TYPE.Ptr(),
            )
    premadeThriftType_i32 = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_I32_TYPE.Ptr(),
            )
    premadeThriftType_i64 = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_I64_TYPE.Ptr(),
            )
    premadeThriftType_float = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_FLOAT_TYPE.Ptr(),
            )
    premadeThriftType_double = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_DOUBLE_TYPE.Ptr(),
            )
    premadeThriftType_string = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_STRING_TYPE.Ptr(),
            )
    premadeThriftType_binary = metadata.NewThriftType().SetTPrimitive(
        metadata.ThriftPrimitiveType_THRIFT_BINARY_TYPE.Ptr(),
            )
    premadeThriftType_standard_ByteBuffer = metadata.NewThriftType().SetTTypedef(
        metadata.NewThriftTypedefType().
            SetName("standard.ByteBuffer").
            SetUnderlyingType(premadeThriftType_binary),
            )
    premadeThriftType_id_FieldId = metadata.NewThriftType().SetTTypedef(
        metadata.NewThriftTypedefType().
            SetName("id.FieldId").
            SetUnderlyingType(premadeThriftType_i16),
            )
    premadeThriftType_list_i16 = metadata.NewThriftType().SetTList(
        metadata.NewThriftListType().
            SetValueType(premadeThriftType_i16),
            )
)

var structMetadatas = []*metadata.ThriftStruct{
    metadata.NewThriftStruct().
    SetName("patch.GeneratePatch").
    SetIsUnion(false),
    metadata.NewThriftStruct().
    SetName("patch.GeneratePatchNew").
    SetIsUnion(false),
    metadata.NewThriftStruct().
    SetName("patch.AssignOnlyPatch").
    SetIsUnion(false),
    metadata.NewThriftStruct().
    SetName("patch.BoolPatch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(9).
    SetName("invert").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.BytePatch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_byte),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("add").
    SetIsOptional(false).
    SetType(premadeThriftType_byte),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.I16Patch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_i16),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("add").
    SetIsOptional(false).
    SetType(premadeThriftType_i16),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.I32Patch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_i32),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("add").
    SetIsOptional(false).
    SetType(premadeThriftType_i32),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.I64Patch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_i64),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("add").
    SetIsOptional(false).
    SetType(premadeThriftType_i64),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.FloatPatch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_float),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("add").
    SetIsOptional(false).
    SetType(premadeThriftType_float),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.DoublePatch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_double),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("add").
    SetIsOptional(false).
    SetType(premadeThriftType_double),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.StringPatch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_string),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("prepend").
    SetIsOptional(false).
    SetType(premadeThriftType_string),
            metadata.NewThriftField().
    SetId(9).
    SetName("append").
    SetIsOptional(false).
    SetType(premadeThriftType_string),
        },
    ),
    metadata.NewThriftStruct().
    SetName("patch.BinaryPatch").
    SetIsUnion(false).
    SetFields(
        []*metadata.ThriftField{
            metadata.NewThriftField().
    SetId(1).
    SetName("assign").
    SetIsOptional(true).
    SetType(premadeThriftType_standard_ByteBuffer),
            metadata.NewThriftField().
    SetId(2).
    SetName("clear").
    SetIsOptional(false).
    SetType(premadeThriftType_bool),
            metadata.NewThriftField().
    SetId(8).
    SetName("prepend").
    SetIsOptional(false).
    SetType(premadeThriftType_standard_ByteBuffer),
            metadata.NewThriftField().
    SetId(9).
    SetName("append").
    SetIsOptional(false).
    SetType(premadeThriftType_standard_ByteBuffer),
        },
    ),
}

var exceptionMetadatas = []*metadata.ThriftException{
}

var enumMetadatas = []*metadata.ThriftEnum{
}

var serviceMetadatas = []*metadata.ThriftService{
}

// GetThriftMetadata returns complete Thrift metadata for current and imported packages.
func GetThriftMetadata() *metadata.ThriftMetadata {
    allEnums := GetEnumsMetadata()
    allStructs := GetStructsMetadata()
    allExceptions := GetExceptionsMetadata()
    allServices := GetServicesMetadata()

    return metadata.NewThriftMetadata().
        SetEnums(allEnums).
        SetStructs(allStructs).
        SetExceptions(allExceptions).
        SetServices(allServices)
}

// GetEnumsMetadata returns Thrift metadata for enums in the current and recursively included packages.
func GetEnumsMetadata() map[string]*metadata.ThriftEnum {
    allEnumsMap := make(map[string]*metadata.ThriftEnum)

    // Add enum metadatas from the current program...
    for _, enumMetadata := range enumMetadatas {
        allEnumsMap[enumMetadata.GetName()] = enumMetadata
    }

    // ...now add enum metadatas from recursively included programs.
    mapsCopy(allEnumsMap, standard.GetEnumsMetadata())
    mapsCopy(allEnumsMap, id.GetEnumsMetadata())

    return allEnumsMap
}

// GetStructsMetadata returns Thrift metadata for structs in the current and recursively included packages.
func GetStructsMetadata() map[string]*metadata.ThriftStruct {
    allStructsMap := make(map[string]*metadata.ThriftStruct)

    // Add struct metadatas from the current program...
    for _, structMetadata := range structMetadatas {
        allStructsMap[structMetadata.GetName()] = structMetadata
    }

    // ...now add struct metadatas from recursively included programs.
    mapsCopy(allStructsMap, standard.GetStructsMetadata())
    mapsCopy(allStructsMap, id.GetStructsMetadata())

    return allStructsMap
}

// GetExceptionsMetadata returns Thrift metadata for exceptions in the current and recursively included packages.
func GetExceptionsMetadata() map[string]*metadata.ThriftException {
    allExceptionsMap := make(map[string]*metadata.ThriftException)

    // Add exception metadatas from the current program...
    for _, exceptionMetadata := range exceptionMetadatas {
        allExceptionsMap[exceptionMetadata.GetName()] = exceptionMetadata
    }

    // ...now add exception metadatas from recursively included programs.
    mapsCopy(allExceptionsMap, standard.GetExceptionsMetadata())
    mapsCopy(allExceptionsMap, id.GetExceptionsMetadata())

    return allExceptionsMap
}

// GetServicesMetadata returns Thrift metadata for services in the current and recursively included packages.
func GetServicesMetadata() map[string]*metadata.ThriftService {
    allServicesMap := make(map[string]*metadata.ThriftService)

    // Add service metadatas from the current program...
    for _, serviceMetadata := range serviceMetadatas {
        allServicesMap[serviceMetadata.GetName()] = serviceMetadata
    }

    // ...now add service metadatas from recursively included programs.
    mapsCopy(allServicesMap, standard.GetServicesMetadata())
    mapsCopy(allServicesMap, id.GetServicesMetadata())

    return allServicesMap
}

// GetThriftMetadataForService returns Thrift metadata for the given service.
func GetThriftMetadataForService(scopedServiceName string) *metadata.ThriftMetadata {
    thriftMetadata := GetThriftMetadata()

    allServicesMap := thriftMetadata.GetServices()
    relevantServicesMap := make(map[string]*metadata.ThriftService)

    serviceMetadata := allServicesMap[scopedServiceName]
    // Visit and record all recursive parents of the target service.
    for serviceMetadata != nil {
        relevantServicesMap[serviceMetadata.GetName()] = serviceMetadata
        if serviceMetadata.IsSetParent() {
            serviceMetadata = allServicesMap[serviceMetadata.GetParent()]
        } else {
            serviceMetadata = nil
        }
    }

    thriftMetadata.SetServices(relevantServicesMap)

    return thriftMetadata
}
