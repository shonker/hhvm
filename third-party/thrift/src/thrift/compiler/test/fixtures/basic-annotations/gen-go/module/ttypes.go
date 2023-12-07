// Autogenerated by Thrift Compiler (facebook)
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
// @generated

package module

import (
	"bytes"
	"context"
	"sync"
	"fmt"
	thrift "github.com/facebook/fbthrift/thrift/lib/go/thrift"
)

// (needed to ensure safety because of naive import list construction.)
var _ = thrift.ZERO
var _ = fmt.Printf
var _ = sync.Mutex{}
var _ = bytes.Equal
var _ = context.Background

var GoUnusedProtection__ int;

type MyEnum int64
const (
  MyEnum_MyValue1 MyEnum = 0
  MyEnum_MyValue2 MyEnum = 1
  MyEnum_DOMAIN MyEnum = 2
)

var MyEnumToName = map[MyEnum]string {
  MyEnum_MyValue1: "MyValue1",
  MyEnum_MyValue2: "MyValue2",
  MyEnum_DOMAIN: "DOMAIN",
}

var MyEnumToValue = map[string]MyEnum {
  "MyValue1": MyEnum_MyValue1,
  "MyValue2": MyEnum_MyValue2,
  "DOMAIN": MyEnum_DOMAIN,
}

var MyEnumNames = []string {
  "MyValue1",
  "MyValue2",
  "DOMAIN",
}

var MyEnumValues = []MyEnum {
  MyEnum_MyValue1,
  MyEnum_MyValue2,
  MyEnum_DOMAIN,
}

func (p MyEnum) String() string {
  if v, ok := MyEnumToName[p]; ok {
    return v
  }
  return "<UNSET>"
}

func MyEnumFromString(s string) (MyEnum, error) {
  if v, ok := MyEnumToValue[s]; ok {
    return v, nil
  }
  return MyEnum(0), fmt.Errorf("not a valid MyEnum string")
}

func MyEnumPtr(v MyEnum) *MyEnum { return &v }

type AwesomeStruct = MyStruct

func AwesomeStructPtr(v AwesomeStruct) *AwesomeStruct { return &v }

func NewAwesomeStruct() *AwesomeStruct { return NewMyStruct() }

type FantasticStruct = MyStruct

func FantasticStructPtr(v FantasticStruct) *FantasticStruct { return &v }

func NewFantasticStruct() *FantasticStruct { return NewMyStruct() }

type ListString_6884 = []string

func ListString_6884Ptr(v ListString_6884) *ListString_6884 { return &v }

// Attributes:
//  - Name
type MyStructNestedAnnotation struct {
  Name string `thrift:"name,1" db:"name" json:"name"`
}

func NewMyStructNestedAnnotation() *MyStructNestedAnnotation {
  return &MyStructNestedAnnotation{}
}


func (p *MyStructNestedAnnotation) GetName() string {
  return p.Name
}
type MyStructNestedAnnotationBuilder struct {
  obj *MyStructNestedAnnotation
}

func NewMyStructNestedAnnotationBuilder() *MyStructNestedAnnotationBuilder{
  return &MyStructNestedAnnotationBuilder{
    obj: NewMyStructNestedAnnotation(),
  }
}

func (p MyStructNestedAnnotationBuilder) Emit() *MyStructNestedAnnotation{
  return &MyStructNestedAnnotation{
    Name: p.obj.Name,
  }
}

func (m *MyStructNestedAnnotationBuilder) Name(name string) *MyStructNestedAnnotationBuilder {
  m.obj.Name = name
  return m
}

func (m *MyStructNestedAnnotation) SetName(name string) *MyStructNestedAnnotation {
  m.Name = name
  return m
}

func (p *MyStructNestedAnnotation) Read(iprot thrift.Protocol) error {
  if _, err := iprot.ReadStructBegin(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read error: ", p), err)
  }


  for {
    _, fieldTypeId, fieldId, err := iprot.ReadFieldBegin()
    if err != nil {
      return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", p, fieldId), err)
    }
    if fieldTypeId == thrift.STOP { break; }
    switch fieldId {
    case 1:
      if err := p.ReadField1(iprot); err != nil {
        return err
      }
    default:
      if err := iprot.Skip(fieldTypeId); err != nil {
        return err
      }
    }
    if err := iprot.ReadFieldEnd(); err != nil {
      return err
    }
  }
  if err := iprot.ReadStructEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read struct end error: ", p), err)
  }
  return nil
}

func (p *MyStructNestedAnnotation)  ReadField1(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 1: ", err)
  } else {
    p.Name = v
  }
  return nil
}

func (p *MyStructNestedAnnotation) Write(oprot thrift.Protocol) error {
  if err := oprot.WriteStructBegin("MyStructNestedAnnotation"); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", p), err) }
  if err := p.writeField1(oprot); err != nil { return err }
  if err := oprot.WriteFieldStop(); err != nil {
    return thrift.PrependError("write field stop error: ", err) }
  if err := oprot.WriteStructEnd(); err != nil {
    return thrift.PrependError("write struct stop error: ", err) }
  return nil
}

func (p *MyStructNestedAnnotation) writeField1(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("name", thrift.STRING, 1); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 1:name: ", p), err) }
  if err := oprot.WriteString(string(p.Name)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.name (1) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 1:name: ", p), err) }
  return err
}

func (p *MyStructNestedAnnotation) String() string {
  if p == nil {
    return "<nil>"
  }

  nameVal := fmt.Sprintf("%v", p.Name)
  return fmt.Sprintf("MyStructNestedAnnotation({Name:%s})", nameVal)
}

type MyUnion struct {
}

func NewMyUnion() *MyUnion {
  return &MyUnion{}
}

type MyUnionBuilder struct {
  obj *MyUnion
}

func NewMyUnionBuilder() *MyUnionBuilder{
  return &MyUnionBuilder{
    obj: NewMyUnion(),
  }
}

func (p MyUnionBuilder) Emit() *MyUnion{
  return &MyUnion{
  }
}

func (p *MyUnion) Read(iprot thrift.Protocol) error {
  if _, err := iprot.ReadStructBegin(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read error: ", p), err)
  }


  for {
    _, fieldTypeId, fieldId, err := iprot.ReadFieldBegin()
    if err != nil {
      return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", p, fieldId), err)
    }
    if fieldTypeId == thrift.STOP { break; }
    if err := iprot.Skip(fieldTypeId); err != nil {
      return err
    }
    if err := iprot.ReadFieldEnd(); err != nil {
      return err
    }
  }
  if err := iprot.ReadStructEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read struct end error: ", p), err)
  }
  return nil
}

func (p *MyUnion) Write(oprot thrift.Protocol) error {
  if err := oprot.WriteStructBegin("MyUnion"); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", p), err) }
  if err := oprot.WriteFieldStop(); err != nil {
    return thrift.PrependError("write field stop error: ", err) }
  if err := oprot.WriteStructEnd(); err != nil {
    return thrift.PrependError("write struct stop error: ", err) }
  return nil
}

func (p *MyUnion) String() string {
  if p == nil {
    return "<nil>"
  }

  return fmt.Sprintf("MyUnion({})")
}

type MyException struct {
}

func NewMyException() *MyException {
  return &MyException{}
}

type MyExceptionBuilder struct {
  obj *MyException
}

func NewMyExceptionBuilder() *MyExceptionBuilder{
  return &MyExceptionBuilder{
    obj: NewMyException(),
  }
}

func (p MyExceptionBuilder) Emit() *MyException{
  return &MyException{
  }
}

func (p *MyException) Read(iprot thrift.Protocol) error {
  if _, err := iprot.ReadStructBegin(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read error: ", p), err)
  }


  for {
    _, fieldTypeId, fieldId, err := iprot.ReadFieldBegin()
    if err != nil {
      return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", p, fieldId), err)
    }
    if fieldTypeId == thrift.STOP { break; }
    if err := iprot.Skip(fieldTypeId); err != nil {
      return err
    }
    if err := iprot.ReadFieldEnd(); err != nil {
      return err
    }
  }
  if err := iprot.ReadStructEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read struct end error: ", p), err)
  }
  return nil
}

func (p *MyException) Write(oprot thrift.Protocol) error {
  if err := oprot.WriteStructBegin("MyException"); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", p), err) }
  if err := oprot.WriteFieldStop(); err != nil {
    return thrift.PrependError("write field stop error: ", err) }
  if err := oprot.WriteStructEnd(); err != nil {
    return thrift.PrependError("write struct stop error: ", err) }
  return nil
}

func (p *MyException) String() string {
  if p == nil {
    return "<nil>"
  }

  return fmt.Sprintf("MyException({})")
}

func (p *MyException) Error() string {
  return p.String()
}

// Attributes:
//  - Major
//  - Abstract
//  - AnnotationWithQuote
//  - Class_
//  - AnnotationWithTrailingComma
//  - EmptyAnnotations
//  - MyEnum
//  - CppTypeAnnotation
//  - MyUnion
type MyStruct struct {
  Abstract string `thrift:"abstract,1" tag:"some_abstract"`
  Major int64 `thrift:"major,2" db:"major" json:"major"`
  AnnotationWithQuote string `thrift:"annotation_with_quote,3" tag:"somevalue"`
  Class_ string `thrift:"class_,4" db:"class_" json:"class_"`
  AnnotationWithTrailingComma string `thrift:"annotation_with_trailing_comma,5" db:"annotation_with_trailing_comma" json:"annotation_with_trailing_comma"`
  EmptyAnnotations string `thrift:"empty_annotations,6" db:"empty_annotations" json:"empty_annotations"`
  MyEnum MyEnum `thrift:"my_enum,7" db:"my_enum" json:"my_enum"`
  CppTypeAnnotation []string `thrift:"cpp_type_annotation,8" db:"cpp_type_annotation" json:"cpp_type_annotation"`
  MyUnion *MyUnion `thrift:"my_union,9" db:"my_union" json:"my_union"`
}

func NewMyStruct() *MyStruct {
  return &MyStruct{}
}


func (p *MyStruct) GetMajor() int64 {
  return p.Major
}

func (p *MyStruct) GetAbstract() string {
  return p.Abstract
}

func (p *MyStruct) GetAnnotationWithQuote() string {
  return p.AnnotationWithQuote
}

func (p *MyStruct) GetClass_() string {
  return p.Class_
}

func (p *MyStruct) GetAnnotationWithTrailingComma() string {
  return p.AnnotationWithTrailingComma
}

func (p *MyStruct) GetEmptyAnnotations() string {
  return p.EmptyAnnotations
}

func (p *MyStruct) GetMyEnum() MyEnum {
  return p.MyEnum
}

func (p *MyStruct) GetCppTypeAnnotation() []string {
  return p.CppTypeAnnotation
}
var MyStruct_MyUnion_DEFAULT *MyUnion
func (p *MyStruct) GetMyUnion() *MyUnion {
  if !p.IsSetMyUnion() {
    return MyStruct_MyUnion_DEFAULT
  }
  return p.MyUnion
}
func (p *MyStruct) DefaultGetMyUnion() *MyUnion {
  if !p.IsSetMyUnion() {
    return NewMyUnion()
  }
  return p.MyUnion
}
func (p *MyStruct) IsSetMyUnion() bool {
  return p != nil && p.MyUnion != nil
}

type MyStructBuilder struct {
  obj *MyStruct
}

func NewMyStructBuilder() *MyStructBuilder{
  return &MyStructBuilder{
    obj: NewMyStruct(),
  }
}

func (p MyStructBuilder) Emit() *MyStruct{
  return &MyStruct{
    Major: p.obj.Major,
    Abstract: p.obj.Abstract,
    AnnotationWithQuote: p.obj.AnnotationWithQuote,
    Class_: p.obj.Class_,
    AnnotationWithTrailingComma: p.obj.AnnotationWithTrailingComma,
    EmptyAnnotations: p.obj.EmptyAnnotations,
    MyEnum: p.obj.MyEnum,
    CppTypeAnnotation: p.obj.CppTypeAnnotation,
    MyUnion: p.obj.MyUnion,
  }
}

func (m *MyStructBuilder) Major(major int64) *MyStructBuilder {
  m.obj.Major = major
  return m
}

func (m *MyStructBuilder) Abstract(abstract string) *MyStructBuilder {
  m.obj.Abstract = abstract
  return m
}

func (m *MyStructBuilder) AnnotationWithQuote(annotationWithQuote string) *MyStructBuilder {
  m.obj.AnnotationWithQuote = annotationWithQuote
  return m
}

func (m *MyStructBuilder) Class_(class_ string) *MyStructBuilder {
  m.obj.Class_ = class_
  return m
}

func (m *MyStructBuilder) AnnotationWithTrailingComma(annotationWithTrailingComma string) *MyStructBuilder {
  m.obj.AnnotationWithTrailingComma = annotationWithTrailingComma
  return m
}

func (m *MyStructBuilder) EmptyAnnotations(emptyAnnotations string) *MyStructBuilder {
  m.obj.EmptyAnnotations = emptyAnnotations
  return m
}

func (m *MyStructBuilder) MyEnum(myEnum MyEnum) *MyStructBuilder {
  m.obj.MyEnum = myEnum
  return m
}

func (m *MyStructBuilder) CppTypeAnnotation(cppTypeAnnotation []string) *MyStructBuilder {
  m.obj.CppTypeAnnotation = cppTypeAnnotation
  return m
}

func (m *MyStructBuilder) MyUnion(myUnion *MyUnion) *MyStructBuilder {
  m.obj.MyUnion = myUnion
  return m
}

func (m *MyStruct) SetMajor(major int64) *MyStruct {
  m.Major = major
  return m
}

func (m *MyStruct) SetAbstract(abstract string) *MyStruct {
  m.Abstract = abstract
  return m
}

func (m *MyStruct) SetAnnotationWithQuote(annotationWithQuote string) *MyStruct {
  m.AnnotationWithQuote = annotationWithQuote
  return m
}

func (m *MyStruct) SetClass_(class_ string) *MyStruct {
  m.Class_ = class_
  return m
}

func (m *MyStruct) SetAnnotationWithTrailingComma(annotationWithTrailingComma string) *MyStruct {
  m.AnnotationWithTrailingComma = annotationWithTrailingComma
  return m
}

func (m *MyStruct) SetEmptyAnnotations(emptyAnnotations string) *MyStruct {
  m.EmptyAnnotations = emptyAnnotations
  return m
}

func (m *MyStruct) SetMyEnum(myEnum MyEnum) *MyStruct {
  m.MyEnum = myEnum
  return m
}

func (m *MyStruct) SetCppTypeAnnotation(cppTypeAnnotation []string) *MyStruct {
  m.CppTypeAnnotation = cppTypeAnnotation
  return m
}

func (m *MyStruct) SetMyUnion(myUnion *MyUnion) *MyStruct {
  m.MyUnion = myUnion
  return m
}

func (p *MyStruct) Read(iprot thrift.Protocol) error {
  if _, err := iprot.ReadStructBegin(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read error: ", p), err)
  }


  for {
    _, fieldTypeId, fieldId, err := iprot.ReadFieldBegin()
    if err != nil {
      return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", p, fieldId), err)
    }
    if fieldTypeId == thrift.STOP { break; }
    switch fieldId {
    case 2:
      if err := p.ReadField2(iprot); err != nil {
        return err
      }
    case 1:
      if err := p.ReadField1(iprot); err != nil {
        return err
      }
    case 3:
      if err := p.ReadField3(iprot); err != nil {
        return err
      }
    case 4:
      if err := p.ReadField4(iprot); err != nil {
        return err
      }
    case 5:
      if err := p.ReadField5(iprot); err != nil {
        return err
      }
    case 6:
      if err := p.ReadField6(iprot); err != nil {
        return err
      }
    case 7:
      if err := p.ReadField7(iprot); err != nil {
        return err
      }
    case 8:
      if err := p.ReadField8(iprot); err != nil {
        return err
      }
    case 9:
      if err := p.ReadField9(iprot); err != nil {
        return err
      }
    default:
      if err := iprot.Skip(fieldTypeId); err != nil {
        return err
      }
    }
    if err := iprot.ReadFieldEnd(); err != nil {
      return err
    }
  }
  if err := iprot.ReadStructEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read struct end error: ", p), err)
  }
  return nil
}

func (p *MyStruct)  ReadField2(iprot thrift.Protocol) error {
  if v, err := iprot.ReadI64(); err != nil {
    return thrift.PrependError("error reading field 2: ", err)
  } else {
    p.Major = v
  }
  return nil
}

func (p *MyStruct)  ReadField1(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 1: ", err)
  } else {
    p.Abstract = v
  }
  return nil
}

func (p *MyStruct)  ReadField3(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 3: ", err)
  } else {
    p.AnnotationWithQuote = v
  }
  return nil
}

func (p *MyStruct)  ReadField4(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 4: ", err)
  } else {
    p.Class_ = v
  }
  return nil
}

func (p *MyStruct)  ReadField5(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 5: ", err)
  } else {
    p.AnnotationWithTrailingComma = v
  }
  return nil
}

func (p *MyStruct)  ReadField6(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 6: ", err)
  } else {
    p.EmptyAnnotations = v
  }
  return nil
}

func (p *MyStruct)  ReadField7(iprot thrift.Protocol) error {
  if v, err := iprot.ReadI32(); err != nil {
    return thrift.PrependError("error reading field 7: ", err)
  } else {
    temp := MyEnum(v)
    p.MyEnum = temp
  }
  return nil
}

func (p *MyStruct)  ReadField8(iprot thrift.Protocol) error {
  _, size, err := iprot.ReadListBegin()
  if err != nil {
    return thrift.PrependError("error reading list begin: ", err)
  }
  tSlice := make([]string, 0, size)
  p.CppTypeAnnotation =  tSlice
  for i := 0; i < size; i ++ {
    var _elem0 string
    if v, err := iprot.ReadString(); err != nil {
      return thrift.PrependError("error reading field 0: ", err)
    } else {
      _elem0 = v
    }
    p.CppTypeAnnotation = append(p.CppTypeAnnotation, _elem0)
  }
  if err := iprot.ReadListEnd(); err != nil {
    return thrift.PrependError("error reading list end: ", err)
  }
  return nil
}

func (p *MyStruct)  ReadField9(iprot thrift.Protocol) error {
  p.MyUnion = NewMyUnion()
  if err := p.MyUnion.Read(iprot); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T error reading struct: ", p.MyUnion), err)
  }
  return nil
}

func (p *MyStruct) Write(oprot thrift.Protocol) error {
  if err := oprot.WriteStructBegin("MyStruct"); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", p), err) }
  if err := p.writeField1(oprot); err != nil { return err }
  if err := p.writeField2(oprot); err != nil { return err }
  if err := p.writeField3(oprot); err != nil { return err }
  if err := p.writeField4(oprot); err != nil { return err }
  if err := p.writeField5(oprot); err != nil { return err }
  if err := p.writeField6(oprot); err != nil { return err }
  if err := p.writeField7(oprot); err != nil { return err }
  if err := p.writeField8(oprot); err != nil { return err }
  if err := p.writeField9(oprot); err != nil { return err }
  if err := oprot.WriteFieldStop(); err != nil {
    return thrift.PrependError("write field stop error: ", err) }
  if err := oprot.WriteStructEnd(); err != nil {
    return thrift.PrependError("write struct stop error: ", err) }
  return nil
}

func (p *MyStruct) writeField1(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("abstract", thrift.STRING, 1); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 1:abstract: ", p), err) }
  if err := oprot.WriteString(string(p.Abstract)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.abstract (1) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 1:abstract: ", p), err) }
  return err
}

func (p *MyStruct) writeField2(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("major", thrift.I64, 2); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 2:major: ", p), err) }
  if err := oprot.WriteI64(int64(p.Major)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.major (2) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 2:major: ", p), err) }
  return err
}

func (p *MyStruct) writeField3(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("annotation_with_quote", thrift.STRING, 3); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 3:annotation_with_quote: ", p), err) }
  if err := oprot.WriteString(string(p.AnnotationWithQuote)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.annotation_with_quote (3) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 3:annotation_with_quote: ", p), err) }
  return err
}

func (p *MyStruct) writeField4(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("class_", thrift.STRING, 4); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 4:class_: ", p), err) }
  if err := oprot.WriteString(string(p.Class_)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.class_ (4) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 4:class_: ", p), err) }
  return err
}

func (p *MyStruct) writeField5(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("annotation_with_trailing_comma", thrift.STRING, 5); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 5:annotation_with_trailing_comma: ", p), err) }
  if err := oprot.WriteString(string(p.AnnotationWithTrailingComma)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.annotation_with_trailing_comma (5) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 5:annotation_with_trailing_comma: ", p), err) }
  return err
}

func (p *MyStruct) writeField6(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("empty_annotations", thrift.STRING, 6); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 6:empty_annotations: ", p), err) }
  if err := oprot.WriteString(string(p.EmptyAnnotations)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.empty_annotations (6) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 6:empty_annotations: ", p), err) }
  return err
}

func (p *MyStruct) writeField7(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("my_enum", thrift.I32, 7); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 7:my_enum: ", p), err) }
  if err := oprot.WriteI32(int32(p.MyEnum)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.my_enum (7) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 7:my_enum: ", p), err) }
  return err
}

func (p *MyStruct) writeField8(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("cpp_type_annotation", thrift.LIST, 8); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 8:cpp_type_annotation: ", p), err) }
  if err := oprot.WriteListBegin(thrift.STRING, len(p.CppTypeAnnotation)); err != nil {
    return thrift.PrependError("error writing list begin: ", err)
  }
  for _, v := range p.CppTypeAnnotation {
    if err := oprot.WriteString(string(v)); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T. (0) field write error: ", p), err) }
  }
  if err := oprot.WriteListEnd(); err != nil {
    return thrift.PrependError("error writing list end: ", err)
  }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 8:cpp_type_annotation: ", p), err) }
  return err
}

func (p *MyStruct) writeField9(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("my_union", thrift.STRUCT, 9); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 9:my_union: ", p), err) }
  if err := p.MyUnion.Write(oprot); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T error writing struct: ", p.MyUnion), err)
  }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 9:my_union: ", p), err) }
  return err
}

func (p *MyStruct) String() string {
  if p == nil {
    return "<nil>"
  }

  abstractVal := fmt.Sprintf("%v", p.Abstract)
  majorVal := fmt.Sprintf("%v", p.Major)
  annotationWithQuoteVal := fmt.Sprintf("%v", p.AnnotationWithQuote)
  class_Val := fmt.Sprintf("%v", p.Class_)
  annotationWithTrailingCommaVal := fmt.Sprintf("%v", p.AnnotationWithTrailingComma)
  emptyAnnotationsVal := fmt.Sprintf("%v", p.EmptyAnnotations)
  myEnumVal := fmt.Sprintf("%v", p.MyEnum)
  cppTypeAnnotationVal := fmt.Sprintf("%v", p.CppTypeAnnotation)
  var myUnionVal string
  if p.MyUnion == nil {
    myUnionVal = "<nil>"
  } else {
    myUnionVal = fmt.Sprintf("%v", p.MyUnion)
  }
  return fmt.Sprintf("MyStruct({Abstract:%s Major:%s AnnotationWithQuote:%s Class_:%s AnnotationWithTrailingComma:%s EmptyAnnotations:%s MyEnum:%s CppTypeAnnotation:%s MyUnion:%s})", abstractVal, majorVal, annotationWithQuoteVal, class_Val, annotationWithTrailingCommaVal, emptyAnnotationsVal, myEnumVal, cppTypeAnnotationVal, myUnionVal)
}

// Attributes:
//  - Id
//  - Password
type SecretStruct struct {
  Id int64 `thrift:"id,1" db:"id" json:"id"`
  Password string `thrift:"password,2" db:"password" json:"password"`
}

func NewSecretStruct() *SecretStruct {
  return &SecretStruct{}
}


func (p *SecretStruct) GetId() int64 {
  return p.Id
}

func (p *SecretStruct) GetPassword() string {
  return p.Password
}
type SecretStructBuilder struct {
  obj *SecretStruct
}

func NewSecretStructBuilder() *SecretStructBuilder{
  return &SecretStructBuilder{
    obj: NewSecretStruct(),
  }
}

func (p SecretStructBuilder) Emit() *SecretStruct{
  return &SecretStruct{
    Id: p.obj.Id,
    Password: p.obj.Password,
  }
}

func (s *SecretStructBuilder) Id(id int64) *SecretStructBuilder {
  s.obj.Id = id
  return s
}

func (s *SecretStructBuilder) Password(password string) *SecretStructBuilder {
  s.obj.Password = password
  return s
}

func (s *SecretStruct) SetId(id int64) *SecretStruct {
  s.Id = id
  return s
}

func (s *SecretStruct) SetPassword(password string) *SecretStruct {
  s.Password = password
  return s
}

func (p *SecretStruct) Read(iprot thrift.Protocol) error {
  if _, err := iprot.ReadStructBegin(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read error: ", p), err)
  }


  for {
    _, fieldTypeId, fieldId, err := iprot.ReadFieldBegin()
    if err != nil {
      return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", p, fieldId), err)
    }
    if fieldTypeId == thrift.STOP { break; }
    switch fieldId {
    case 1:
      if err := p.ReadField1(iprot); err != nil {
        return err
      }
    case 2:
      if err := p.ReadField2(iprot); err != nil {
        return err
      }
    default:
      if err := iprot.Skip(fieldTypeId); err != nil {
        return err
      }
    }
    if err := iprot.ReadFieldEnd(); err != nil {
      return err
    }
  }
  if err := iprot.ReadStructEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T read struct end error: ", p), err)
  }
  return nil
}

func (p *SecretStruct)  ReadField1(iprot thrift.Protocol) error {
  if v, err := iprot.ReadI64(); err != nil {
    return thrift.PrependError("error reading field 1: ", err)
  } else {
    p.Id = v
  }
  return nil
}

func (p *SecretStruct)  ReadField2(iprot thrift.Protocol) error {
  if v, err := iprot.ReadString(); err != nil {
    return thrift.PrependError("error reading field 2: ", err)
  } else {
    p.Password = v
  }
  return nil
}

func (p *SecretStruct) Write(oprot thrift.Protocol) error {
  if err := oprot.WriteStructBegin("SecretStruct"); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", p), err) }
  if err := p.writeField1(oprot); err != nil { return err }
  if err := p.writeField2(oprot); err != nil { return err }
  if err := oprot.WriteFieldStop(); err != nil {
    return thrift.PrependError("write field stop error: ", err) }
  if err := oprot.WriteStructEnd(); err != nil {
    return thrift.PrependError("write struct stop error: ", err) }
  return nil
}

func (p *SecretStruct) writeField1(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("id", thrift.I64, 1); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 1:id: ", p), err) }
  if err := oprot.WriteI64(int64(p.Id)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.id (1) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 1:id: ", p), err) }
  return err
}

func (p *SecretStruct) writeField2(oprot thrift.Protocol) (err error) {
  if err := oprot.WriteFieldBegin("password", thrift.STRING, 2); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field begin error 2:password: ", p), err) }
  if err := oprot.WriteString(string(p.Password)); err != nil {
  return thrift.PrependError(fmt.Sprintf("%T.password (2) field write error: ", p), err) }
  if err := oprot.WriteFieldEnd(); err != nil {
    return thrift.PrependError(fmt.Sprintf("%T write field end error 2:password: ", p), err) }
  return err
}

func (p *SecretStruct) String() string {
  if p == nil {
    return "<nil>"
  }

  idVal := fmt.Sprintf("%v", p.Id)
  passwordVal := fmt.Sprintf("%v", p.Password)
  return fmt.Sprintf("SecretStruct({Id:%s Password:%s})", idVal, passwordVal)
}

