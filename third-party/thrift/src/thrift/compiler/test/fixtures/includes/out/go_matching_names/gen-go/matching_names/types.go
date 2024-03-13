// @generated by Thrift for [[[ program path ]]]
// This file is probably not the place you want to edit!

package matching_names // [[[ program thrift source path ]]]

import (
    "fmt"
    "strings"

    includesAlso "IncludesAlso"
    thrift "github.com/facebook/fbthrift/thrift/lib/go/thrift"
)

var _ = includesAlso.GoUnusedProtection__
// (needed to ensure safety because of naive import list construction)
var _ = fmt.Printf
var _ = strings.Split
var _ = thrift.ZERO


type IncludesAlso struct {
    Also *includesAlso.Also `thrift:"also,1" json:"also" db:"also"`
}
// Compile time interface enforcer
var _ thrift.Struct = &IncludesAlso{}

func NewIncludesAlso() *IncludesAlso {
    return (&IncludesAlso{}).
        SetAlsoNonCompat(*includesAlso.NewAlso())
}

func (x *IncludesAlso) GetAlsoNonCompat() *includesAlso.Also {
    return x.Also
}

func (x *IncludesAlso) GetAlso() *includesAlso.Also {
    if !x.IsSetAlso() {
        return nil
    }

    return x.Also
}

func (x *IncludesAlso) SetAlsoNonCompat(value includesAlso.Also) *IncludesAlso {
    x.Also = &value
    return x
}

func (x *IncludesAlso) SetAlso(value *includesAlso.Also) *IncludesAlso {
    x.Also = value
    return x
}

func (x *IncludesAlso) IsSetAlso() bool {
    return x != nil && x.Also != nil
}

func (x *IncludesAlso) writeField1(p thrift.Format) error {  // Also
    if !x.IsSetAlso() {
        return nil
    }

    if err := p.WriteFieldBegin("also", thrift.STRUCT, 1); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field begin error: ", x), err)
    }

    item := x.GetAlsoNonCompat()
    if err := item.Write(p); err != nil {
    return err
}

    if err := p.WriteFieldEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field end error: ", x), err)
    }
    return nil
}

func (x *IncludesAlso) readField1(p thrift.Format) error {  // Also
    result := *includesAlso.NewAlso()
err := result.Read(p)
if err != nil {
    return err
}

    x.SetAlsoNonCompat(result)
    return nil
}

func (x *IncludesAlso) toString1() string {  // Also
    return fmt.Sprintf("%v", x.GetAlsoNonCompat())
}

// Deprecated: Use NewIncludesAlso().GetAlso() instead.
func (x *IncludesAlso) DefaultGetAlso() *includesAlso.Also {
    if !x.IsSetAlso() {
        return includesAlso.NewAlso()
    }
    return x.Also
}


// Deprecated: Use "New" constructor and setters to build your structs.
// e.g NewIncludesAlso().Set<FieldNameFoo>().Set<FieldNameBar>()
type IncludesAlsoBuilder struct {
    obj *IncludesAlso
}

// Deprecated: Use "New" constructor and setters to build your structs.
// e.g NewIncludesAlso().Set<FieldNameFoo>().Set<FieldNameBar>()
func NewIncludesAlsoBuilder() *IncludesAlsoBuilder {
    return &IncludesAlsoBuilder{
        obj: NewIncludesAlso(),
    }
}

// Deprecated: Use "New" constructor and setters to build your structs.
// e.g NewIncludesAlso().Set<FieldNameFoo>().Set<FieldNameBar>()
func (x *IncludesAlsoBuilder) Also(value *includesAlso.Also) *IncludesAlsoBuilder {
    x.obj.Also = value
    return x
}

// Deprecated: Use "New" constructor and setters to build your structs.
// e.g NewIncludesAlso().Set<FieldNameFoo>().Set<FieldNameBar>()
func (x *IncludesAlsoBuilder) Emit() *IncludesAlso {
    var objCopy IncludesAlso = *x.obj
    return &objCopy
}

func (x *IncludesAlso) Write(p thrift.Format) error {
    if err := p.WriteStructBegin("IncludesAlso"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := x.writeField1(p); err != nil {
        return err
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *IncludesAlso) Read(p thrift.Format) error {
    if _, err := p.ReadStructBegin(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T read error: ", x), err)
    }

    for {
        _, wireType, id, err := p.ReadFieldBegin()
        if err != nil {
            return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", x, id), err)
        }

        if wireType == thrift.STOP {
            break;
        }

        switch {
        case (id == 1 && wireType == thrift.Type(thrift.STRUCT)):  // also
            if err := x.readField1(p); err != nil {
                return err
            }
        default:
            if err := p.Skip(wireType); err != nil {
                return err
            }
        }

        if err := p.ReadFieldEnd(); err != nil {
            return err
        }
    }

    if err := p.ReadStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T read struct end error: ", x), err)
    }

    return nil
}

func (x *IncludesAlso) String() string {
    if x == nil {
        return "<nil>"
    }

    var sb strings.Builder

    sb.WriteString("IncludesAlso({")
    sb.WriteString(fmt.Sprintf("Also:%s", x.toString1()))
    sb.WriteString("})")

    return sb.String()
}

// RegisterTypes registers types found in this file that have a thrift_uri with the passed in registry.
func RegisterTypes(registry interface {
  RegisterType(name string, initializer func() any)
}) {

}
