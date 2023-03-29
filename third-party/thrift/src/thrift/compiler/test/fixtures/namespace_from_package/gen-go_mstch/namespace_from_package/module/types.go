// @generated by Thrift for [[[ program path ]]]
// This file is probably not the place you want to edit!

package module // [[[ program thrift source path ]]]

import (
    "fmt"

    thrift "github.com/facebook/fbthrift/thrift/lib/go/thrift"
)


// (needed to ensure safety because of naive import list construction)
var _ = fmt.Printf
var _ = thrift.ZERO


type Foo struct {
    MyInt int64 `thrift:"MyInt,1" json:"MyInt" db:"MyInt"`
}
// Compile time interface enforcer
var _ thrift.Struct = &Foo{}


func NewFoo() *Foo {
    return (&Foo{})
}

func (x *Foo) GetMyIntNonCompat() int64 {
    return x.MyInt
}

func (x *Foo) GetMyInt() int64 {
    return x.MyInt
}

func (x *Foo) SetMyInt(value int64) *Foo {
    x.MyInt = value
    return x
}


func (x *Foo) writeField1(p thrift.Protocol) error {  // MyInt
    if err := p.WriteFieldBegin("MyInt", thrift.I64, 1); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field begin error: ", x), err)
    }

    item := x.GetMyIntNonCompat()
    if err := p.WriteI64(item); err != nil {
    return err
}

    if err := p.WriteFieldEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field end error: ", x), err)
    }
    return nil
}

func (x *Foo) readField1(p thrift.Protocol) error {  // MyInt
    result, err := p.ReadI64()
if err != nil {
    return err
}

    x.SetMyInt(result)
    return nil
}

func (x *Foo) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use Foo.Set* methods instead or set the fields directly.
type FooBuilder struct {
    obj *Foo
}

func NewFooBuilder() *FooBuilder {
    return &FooBuilder{
        obj: NewFoo(),
    }
}

func (x *FooBuilder) MyInt(value int64) *FooBuilder {
    x.obj.MyInt = value
    return x
}

func (x *FooBuilder) Emit() *Foo {
    var objCopy Foo = *x.obj
    return &objCopy
}

func (x *Foo) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("Foo"); err != nil {
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

func (x *Foo) Read(p thrift.Protocol) error {
    if _, err := p.ReadStructBegin(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T read error: ", x), err)
    }

    for {
        _, typ, id, err := p.ReadFieldBegin()
        if err != nil {
            return thrift.PrependError(fmt.Sprintf("%T field %d read error: ", x, id), err)
        }

        if typ == thrift.STOP {
            break;
        }

        switch id {
        case 1:  // MyInt
            if err := x.readField1(p); err != nil {
                return err
            }
        default:
            if err := p.Skip(typ); err != nil {
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

