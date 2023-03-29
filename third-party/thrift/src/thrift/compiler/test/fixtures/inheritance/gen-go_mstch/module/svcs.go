// @generated by Thrift for [[[ program path ]]]
// This file is probably not the place you want to edit!

package module // [[[ program thrift source path ]]]


import (
    "context"
    "fmt"


    "thrift/lib/go/thrift"
)


// (needed to ensure safety because of naive import list construction)
var _ = context.Background
var _ = fmt.Printf
var _ = thrift.ZERO



type MyRoot interface {
    DoRoot(ctx context.Context) error
}

// Deprecated: Use MyRoot instead.
type MyRootClientInterface interface {
    thrift.ClientInterface
    DoRoot() error
}

type MyRootChannelClient struct {
    ch thrift.RequestChannel
}
// Compile time interface enforcer
var _ MyRoot = &MyRootChannelClient{}

func NewMyRootChannelClient(channel thrift.RequestChannel) *MyRootChannelClient {
    return &MyRootChannelClient{
        ch: channel,
    }
}

func (c *MyRootChannelClient) Close() error {
    return c.ch.Close()
}

func (c *MyRootChannelClient) IsOpen() bool {
    return c.ch.IsOpen()
}

func (c *MyRootChannelClient) Open() error {
    return c.ch.Open()
}

// Deprecated: Use MyRootChannelClient instead.
type MyRootClient struct {
    chClient *MyRootChannelClient
}
// Compile time interface enforcer
var _ MyRootClientInterface = &MyRootClient{}

// Deprecated: Use NewMyRootChannelClient() instead.
func NewMyRootClient(t thrift.Transport, iprot thrift.Protocol, oprot thrift.Protocol) *MyRootClient {
    return &MyRootClient{
        chClient: NewMyRootChannelClient(
            thrift.NewSerialChannel(iprot),
        ),
    }
}

func (c *MyRootClient) Close() error {
    return c.chClient.Close()
}

func (c *MyRootClient) IsOpen() bool {
    return c.chClient.IsOpen()
}

func (c *MyRootClient) Open() error {
    return c.chClient.Open()
}

// Deprecated: Use MyRootChannelClient instead.
type MyRootThreadsafeClient = MyRootClient

// Deprecated: Use NewMyRootChannelClient() instead.
func NewMyRootThreadsafeClient(t thrift.Transport, iprot thrift.Protocol, oprot thrift.Protocol) *MyRootThreadsafeClient {
    return NewMyRootClient(t, iprot, oprot)
}

// Deprecated: Use NewMyRootChannelClient() instead.
func NewMyRootClientProtocol(prot thrift.Protocol) *MyRootClient {
  return NewMyRootClient(prot.Transport(), prot, prot)
}

// Deprecated: Use NewMyRootChannelClient() instead.
func NewMyRootThreadsafeClientProtocol(prot thrift.Protocol) *MyRootClient {
  return NewMyRootClient(prot.Transport(), prot, prot)
}

// Deprecated: Use NewMyRootChannelClient() instead.
func NewMyRootClientFactory(t thrift.Transport, pf thrift.ProtocolFactory) *MyRootClient {
  iprot := pf.GetProtocol(t)
  oprot := pf.GetProtocol(t)
  return NewMyRootClient(t, iprot, oprot)
}

// Deprecated: Use NewMyRootChannelClient() instead.
func NewMyRootThreadsafeClientFactory(t thrift.Transport, pf thrift.ProtocolFactory) *MyRootThreadsafeClient {
  return NewMyRootClientFactory(t, pf)
}


func (c *MyRootChannelClient) DoRoot(ctx context.Context) error {
    in := &reqMyRootDoRoot{
    }
    out := newRespMyRootDoRoot()
    err := c.ch.Call(ctx, "do_root", in, out)
    return err
}

func (c *MyRootClient) DoRoot() error {
    return c.chClient.DoRoot(nil)
}


type reqMyRootDoRoot struct {
}
// Compile time interface enforcer
var _ thrift.Struct = &reqMyRootDoRoot{}


func newReqMyRootDoRoot() *reqMyRootDoRoot {
    return (&reqMyRootDoRoot{})
}

func (x *reqMyRootDoRoot) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use reqMyRootDoRoot.Set* methods instead or set the fields directly.
type reqMyRootDoRootBuilder struct {
    obj *reqMyRootDoRoot
}

func newReqMyRootDoRootBuilder() *reqMyRootDoRootBuilder {
    return &reqMyRootDoRootBuilder{
        obj: newReqMyRootDoRoot(),
    }
}

func (x *reqMyRootDoRootBuilder) Emit() *reqMyRootDoRoot {
    var objCopy reqMyRootDoRoot = *x.obj
    return &objCopy
}

func (x *reqMyRootDoRoot) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("reqMyRootDoRoot"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *reqMyRootDoRoot) Read(p thrift.Protocol) error {
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

type respMyRootDoRoot struct {
}
// Compile time interface enforcer
var _ thrift.Struct = &respMyRootDoRoot{}
var _ thrift.WritableResult = &respMyRootDoRoot{}


func newRespMyRootDoRoot() *respMyRootDoRoot {
    return (&respMyRootDoRoot{})
}

func (x *respMyRootDoRoot) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use respMyRootDoRoot.Set* methods instead or set the fields directly.
type respMyRootDoRootBuilder struct {
    obj *respMyRootDoRoot
}

func newRespMyRootDoRootBuilder() *respMyRootDoRootBuilder {
    return &respMyRootDoRootBuilder{
        obj: newRespMyRootDoRoot(),
    }
}

func (x *respMyRootDoRootBuilder) Emit() *respMyRootDoRoot {
    var objCopy respMyRootDoRoot = *x.obj
    return &objCopy
}

func (x *respMyRootDoRoot) Exception() thrift.WritableException {
    return nil
}

func (x *respMyRootDoRoot) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("respMyRootDoRoot"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *respMyRootDoRoot) Read(p thrift.Protocol) error {
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



type MyRootProcessor struct {
    processorMap       map[string]thrift.ProcessorFunction
    functionServiceMap map[string]string
    handler            MyRoot
}
// Compile time interface enforcer
var _ thrift.Processor = &MyRootProcessor{}

func (p *MyRootProcessor) AddToProcessorMap(key string, processor thrift.ProcessorFunction) {
    p.processorMap[key] = processor
}

func (p *MyRootProcessor) AddToFunctionServiceMap(key, service string) {
    p.functionServiceMap[key] = service
}

func (p *MyRootProcessor) GetProcessorFunction(key string) (processor thrift.ProcessorFunction, err error) {
    if processor, ok := p.processorMap[key]; ok {
        return processor, nil
    }
    return nil, nil
}

func (p *MyRootProcessor) ProcessorMap() map[string]thrift.ProcessorFunction {
    return p.processorMap
}

func (p *MyRootProcessor) FunctionServiceMap() map[string]string {
    return p.functionServiceMap
}

func NewMyRootProcessor(handler MyRoot) *MyRootProcessor {
    p := &MyRootProcessor{
        handler:            handler,
        processorMap:       make(map[string]thrift.ProcessorFunction),
        functionServiceMap: make(map[string]string),
    }
    p.AddToProcessorMap("do_root", &procFuncMyRootDoRoot{handler: handler})
    p.AddToFunctionServiceMap("do_root", "MyRoot")

    return p
}


type procFuncMyRootDoRoot struct {
    handler MyRoot
}
// Compile time interface enforcer
var _ thrift.ProcessorFunction = &procFuncMyRootDoRoot{}

func (p *procFuncMyRootDoRoot) Read(iprot thrift.Protocol) (thrift.Struct, thrift.Exception) {
    args := newReqMyRootDoRoot()
    if err := args.Read(iprot); err != nil {
        return nil, err
    }
    iprot.ReadMessageEnd()
    return args, nil
}

func (p *procFuncMyRootDoRoot) Write(seqId int32, result thrift.WritableStruct, oprot thrift.Protocol) (err thrift.Exception) {
    var err2 error
    messageType := thrift.REPLY
    if _, ok := result.(thrift.ApplicationException); ok {
        messageType = thrift.EXCEPTION
    }
    if err2 = oprot.WriteMessageBegin("DoRoot", messageType, seqId); err2 != nil {
        err = err2
    }
    if err2 = result.Write(oprot); err == nil && err2 != nil {
        err = err2
    }
    if err2 = oprot.WriteMessageEnd(); err == nil && err2 != nil {
        err = err2
    }
    if err2 = oprot.Flush(); err == nil && err2 != nil {
        err = err2
    }
    return err
}

func (p *procFuncMyRootDoRoot) Run(reqStruct thrift.Struct) (thrift.WritableStruct, thrift.ApplicationException) {
    result := newRespMyRootDoRoot()
    err := p.handler.DoRoot()
    if err != nil {
        x := thrift.NewApplicationExceptionCause(thrift.INTERNAL_ERROR, "Internal error processing DoRoot: " + err.Error(), err)
        return x, x
    }

    return result, nil
}




type MyNode interface {
    // Inherited/extended service
    MyRoot

    DoMid(ctx context.Context) error
}

// Deprecated: Use MyNode instead.
type MyNodeClientInterface interface {
    thrift.ClientInterface
    DoMid() error
}

type MyNodeChannelClient struct {
    // Inherited/extended service
    *MyRootChannelClient
    ch thrift.RequestChannel
}
// Compile time interface enforcer
var _ MyNode = &MyNodeChannelClient{}

func NewMyNodeChannelClient(channel thrift.RequestChannel) *MyNodeChannelClient {
    return &MyNodeChannelClient{
        MyRootChannelClient: NewMyRootChannelClient(channel),
        ch: channel,
    }
}

func (c *MyNodeChannelClient) Close() error {
    return c.ch.Close()
}

func (c *MyNodeChannelClient) IsOpen() bool {
    return c.ch.IsOpen()
}

func (c *MyNodeChannelClient) Open() error {
    return c.ch.Open()
}

// Deprecated: Use MyNodeChannelClient instead.
type MyNodeClient struct {
    // Inherited/extended service
    *MyRootClient
    chClient *MyNodeChannelClient
}
// Compile time interface enforcer
var _ MyNodeClientInterface = &MyNodeClient{}

// Deprecated: Use NewMyNodeChannelClient() instead.
func NewMyNodeClient(t thrift.Transport, iprot thrift.Protocol, oprot thrift.Protocol) *MyNodeClient {
    return &MyNodeClient{
        MyRootClient: NewMyRootClient(t, iprot, oprot),
        chClient: NewMyNodeChannelClient(
            thrift.NewSerialChannel(iprot),
        ),
    }
}

func (c *MyNodeClient) Close() error {
    return c.chClient.Close()
}

func (c *MyNodeClient) IsOpen() bool {
    return c.chClient.IsOpen()
}

func (c *MyNodeClient) Open() error {
    return c.chClient.Open()
}

// Deprecated: Use MyNodeChannelClient instead.
type MyNodeThreadsafeClient = MyNodeClient

// Deprecated: Use NewMyNodeChannelClient() instead.
func NewMyNodeThreadsafeClient(t thrift.Transport, iprot thrift.Protocol, oprot thrift.Protocol) *MyNodeThreadsafeClient {
    return NewMyNodeClient(t, iprot, oprot)
}

// Deprecated: Use NewMyNodeChannelClient() instead.
func NewMyNodeClientProtocol(prot thrift.Protocol) *MyNodeClient {
  return NewMyNodeClient(prot.Transport(), prot, prot)
}

// Deprecated: Use NewMyNodeChannelClient() instead.
func NewMyNodeThreadsafeClientProtocol(prot thrift.Protocol) *MyNodeClient {
  return NewMyNodeClient(prot.Transport(), prot, prot)
}

// Deprecated: Use NewMyNodeChannelClient() instead.
func NewMyNodeClientFactory(t thrift.Transport, pf thrift.ProtocolFactory) *MyNodeClient {
  iprot := pf.GetProtocol(t)
  oprot := pf.GetProtocol(t)
  return NewMyNodeClient(t, iprot, oprot)
}

// Deprecated: Use NewMyNodeChannelClient() instead.
func NewMyNodeThreadsafeClientFactory(t thrift.Transport, pf thrift.ProtocolFactory) *MyNodeThreadsafeClient {
  return NewMyNodeClientFactory(t, pf)
}


func (c *MyNodeChannelClient) DoMid(ctx context.Context) error {
    in := &reqMyNodeDoMid{
    }
    out := newRespMyNodeDoMid()
    err := c.ch.Call(ctx, "do_mid", in, out)
    return err
}

func (c *MyNodeClient) DoMid() error {
    return c.chClient.DoMid(nil)
}


type reqMyNodeDoMid struct {
}
// Compile time interface enforcer
var _ thrift.Struct = &reqMyNodeDoMid{}


func newReqMyNodeDoMid() *reqMyNodeDoMid {
    return (&reqMyNodeDoMid{})
}

func (x *reqMyNodeDoMid) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use reqMyNodeDoMid.Set* methods instead or set the fields directly.
type reqMyNodeDoMidBuilder struct {
    obj *reqMyNodeDoMid
}

func newReqMyNodeDoMidBuilder() *reqMyNodeDoMidBuilder {
    return &reqMyNodeDoMidBuilder{
        obj: newReqMyNodeDoMid(),
    }
}

func (x *reqMyNodeDoMidBuilder) Emit() *reqMyNodeDoMid {
    var objCopy reqMyNodeDoMid = *x.obj
    return &objCopy
}

func (x *reqMyNodeDoMid) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("reqMyNodeDoMid"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *reqMyNodeDoMid) Read(p thrift.Protocol) error {
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

type respMyNodeDoMid struct {
}
// Compile time interface enforcer
var _ thrift.Struct = &respMyNodeDoMid{}
var _ thrift.WritableResult = &respMyNodeDoMid{}


func newRespMyNodeDoMid() *respMyNodeDoMid {
    return (&respMyNodeDoMid{})
}

func (x *respMyNodeDoMid) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use respMyNodeDoMid.Set* methods instead or set the fields directly.
type respMyNodeDoMidBuilder struct {
    obj *respMyNodeDoMid
}

func newRespMyNodeDoMidBuilder() *respMyNodeDoMidBuilder {
    return &respMyNodeDoMidBuilder{
        obj: newRespMyNodeDoMid(),
    }
}

func (x *respMyNodeDoMidBuilder) Emit() *respMyNodeDoMid {
    var objCopy respMyNodeDoMid = *x.obj
    return &objCopy
}

func (x *respMyNodeDoMid) Exception() thrift.WritableException {
    return nil
}

func (x *respMyNodeDoMid) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("respMyNodeDoMid"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *respMyNodeDoMid) Read(p thrift.Protocol) error {
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



type MyNodeProcessor struct {
    // Inherited/extended processor
    *MyRootProcessor
}
// Compile time interface enforcer
var _ thrift.Processor = &MyNodeProcessor{}


func NewMyNodeProcessor(handler MyNode) *MyNodeProcessor {
    p := &MyNodeProcessor{
        NewMyRootProcessor(handler),
    }
    p.AddToProcessorMap("do_mid", &procFuncMyNodeDoMid{handler: handler})
    p.AddToFunctionServiceMap("do_mid", "MyNode")

    return p
}


type procFuncMyNodeDoMid struct {
    handler MyNode
}
// Compile time interface enforcer
var _ thrift.ProcessorFunction = &procFuncMyNodeDoMid{}

func (p *procFuncMyNodeDoMid) Read(iprot thrift.Protocol) (thrift.Struct, thrift.Exception) {
    args := newReqMyNodeDoMid()
    if err := args.Read(iprot); err != nil {
        return nil, err
    }
    iprot.ReadMessageEnd()
    return args, nil
}

func (p *procFuncMyNodeDoMid) Write(seqId int32, result thrift.WritableStruct, oprot thrift.Protocol) (err thrift.Exception) {
    var err2 error
    messageType := thrift.REPLY
    if _, ok := result.(thrift.ApplicationException); ok {
        messageType = thrift.EXCEPTION
    }
    if err2 = oprot.WriteMessageBegin("DoMid", messageType, seqId); err2 != nil {
        err = err2
    }
    if err2 = result.Write(oprot); err == nil && err2 != nil {
        err = err2
    }
    if err2 = oprot.WriteMessageEnd(); err == nil && err2 != nil {
        err = err2
    }
    if err2 = oprot.Flush(); err == nil && err2 != nil {
        err = err2
    }
    return err
}

func (p *procFuncMyNodeDoMid) Run(reqStruct thrift.Struct) (thrift.WritableStruct, thrift.ApplicationException) {
    result := newRespMyNodeDoMid()
    err := p.handler.DoMid()
    if err != nil {
        x := thrift.NewApplicationExceptionCause(thrift.INTERNAL_ERROR, "Internal error processing DoMid: " + err.Error(), err)
        return x, x
    }

    return result, nil
}




type MyLeaf interface {
    // Inherited/extended service
    MyNode

    DoLeaf(ctx context.Context) error
}

// Deprecated: Use MyLeaf instead.
type MyLeafClientInterface interface {
    thrift.ClientInterface
    DoLeaf() error
}

type MyLeafChannelClient struct {
    // Inherited/extended service
    *MyNodeChannelClient
    ch thrift.RequestChannel
}
// Compile time interface enforcer
var _ MyLeaf = &MyLeafChannelClient{}

func NewMyLeafChannelClient(channel thrift.RequestChannel) *MyLeafChannelClient {
    return &MyLeafChannelClient{
        MyNodeChannelClient: NewMyNodeChannelClient(channel),
        ch: channel,
    }
}

func (c *MyLeafChannelClient) Close() error {
    return c.ch.Close()
}

func (c *MyLeafChannelClient) IsOpen() bool {
    return c.ch.IsOpen()
}

func (c *MyLeafChannelClient) Open() error {
    return c.ch.Open()
}

// Deprecated: Use MyLeafChannelClient instead.
type MyLeafClient struct {
    // Inherited/extended service
    *MyNodeClient
    chClient *MyLeafChannelClient
}
// Compile time interface enforcer
var _ MyLeafClientInterface = &MyLeafClient{}

// Deprecated: Use NewMyLeafChannelClient() instead.
func NewMyLeafClient(t thrift.Transport, iprot thrift.Protocol, oprot thrift.Protocol) *MyLeafClient {
    return &MyLeafClient{
        MyNodeClient: NewMyNodeClient(t, iprot, oprot),
        chClient: NewMyLeafChannelClient(
            thrift.NewSerialChannel(iprot),
        ),
    }
}

func (c *MyLeafClient) Close() error {
    return c.chClient.Close()
}

func (c *MyLeafClient) IsOpen() bool {
    return c.chClient.IsOpen()
}

func (c *MyLeafClient) Open() error {
    return c.chClient.Open()
}

// Deprecated: Use MyLeafChannelClient instead.
type MyLeafThreadsafeClient = MyLeafClient

// Deprecated: Use NewMyLeafChannelClient() instead.
func NewMyLeafThreadsafeClient(t thrift.Transport, iprot thrift.Protocol, oprot thrift.Protocol) *MyLeafThreadsafeClient {
    return NewMyLeafClient(t, iprot, oprot)
}

// Deprecated: Use NewMyLeafChannelClient() instead.
func NewMyLeafClientProtocol(prot thrift.Protocol) *MyLeafClient {
  return NewMyLeafClient(prot.Transport(), prot, prot)
}

// Deprecated: Use NewMyLeafChannelClient() instead.
func NewMyLeafThreadsafeClientProtocol(prot thrift.Protocol) *MyLeafClient {
  return NewMyLeafClient(prot.Transport(), prot, prot)
}

// Deprecated: Use NewMyLeafChannelClient() instead.
func NewMyLeafClientFactory(t thrift.Transport, pf thrift.ProtocolFactory) *MyLeafClient {
  iprot := pf.GetProtocol(t)
  oprot := pf.GetProtocol(t)
  return NewMyLeafClient(t, iprot, oprot)
}

// Deprecated: Use NewMyLeafChannelClient() instead.
func NewMyLeafThreadsafeClientFactory(t thrift.Transport, pf thrift.ProtocolFactory) *MyLeafThreadsafeClient {
  return NewMyLeafClientFactory(t, pf)
}


func (c *MyLeafChannelClient) DoLeaf(ctx context.Context) error {
    in := &reqMyLeafDoLeaf{
    }
    out := newRespMyLeafDoLeaf()
    err := c.ch.Call(ctx, "do_leaf", in, out)
    return err
}

func (c *MyLeafClient) DoLeaf() error {
    return c.chClient.DoLeaf(nil)
}


type reqMyLeafDoLeaf struct {
}
// Compile time interface enforcer
var _ thrift.Struct = &reqMyLeafDoLeaf{}


func newReqMyLeafDoLeaf() *reqMyLeafDoLeaf {
    return (&reqMyLeafDoLeaf{})
}

func (x *reqMyLeafDoLeaf) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use reqMyLeafDoLeaf.Set* methods instead or set the fields directly.
type reqMyLeafDoLeafBuilder struct {
    obj *reqMyLeafDoLeaf
}

func newReqMyLeafDoLeafBuilder() *reqMyLeafDoLeafBuilder {
    return &reqMyLeafDoLeafBuilder{
        obj: newReqMyLeafDoLeaf(),
    }
}

func (x *reqMyLeafDoLeafBuilder) Emit() *reqMyLeafDoLeaf {
    var objCopy reqMyLeafDoLeaf = *x.obj
    return &objCopy
}

func (x *reqMyLeafDoLeaf) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("reqMyLeafDoLeaf"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *reqMyLeafDoLeaf) Read(p thrift.Protocol) error {
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

type respMyLeafDoLeaf struct {
}
// Compile time interface enforcer
var _ thrift.Struct = &respMyLeafDoLeaf{}
var _ thrift.WritableResult = &respMyLeafDoLeaf{}


func newRespMyLeafDoLeaf() *respMyLeafDoLeaf {
    return (&respMyLeafDoLeaf{})
}

func (x *respMyLeafDoLeaf) String() string {
    return fmt.Sprintf("%+v", x)
}


// Deprecated: Use respMyLeafDoLeaf.Set* methods instead or set the fields directly.
type respMyLeafDoLeafBuilder struct {
    obj *respMyLeafDoLeaf
}

func newRespMyLeafDoLeafBuilder() *respMyLeafDoLeafBuilder {
    return &respMyLeafDoLeafBuilder{
        obj: newRespMyLeafDoLeaf(),
    }
}

func (x *respMyLeafDoLeafBuilder) Emit() *respMyLeafDoLeaf {
    var objCopy respMyLeafDoLeaf = *x.obj
    return &objCopy
}

func (x *respMyLeafDoLeaf) Exception() thrift.WritableException {
    return nil
}

func (x *respMyLeafDoLeaf) Write(p thrift.Protocol) error {
    if err := p.WriteStructBegin("respMyLeafDoLeaf"); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct begin error: ", x), err)
    }

    if err := p.WriteFieldStop(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write field stop error: ", x), err)
    }

    if err := p.WriteStructEnd(); err != nil {
        return thrift.PrependError(fmt.Sprintf("%T write struct end error: ", x), err)
    }
    return nil
}

func (x *respMyLeafDoLeaf) Read(p thrift.Protocol) error {
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



type MyLeafProcessor struct {
    // Inherited/extended processor
    *MyNodeProcessor
}
// Compile time interface enforcer
var _ thrift.Processor = &MyLeafProcessor{}


func NewMyLeafProcessor(handler MyLeaf) *MyLeafProcessor {
    p := &MyLeafProcessor{
        NewMyNodeProcessor(handler),
    }
    p.AddToProcessorMap("do_leaf", &procFuncMyLeafDoLeaf{handler: handler})
    p.AddToFunctionServiceMap("do_leaf", "MyLeaf")

    return p
}


type procFuncMyLeafDoLeaf struct {
    handler MyLeaf
}
// Compile time interface enforcer
var _ thrift.ProcessorFunction = &procFuncMyLeafDoLeaf{}

func (p *procFuncMyLeafDoLeaf) Read(iprot thrift.Protocol) (thrift.Struct, thrift.Exception) {
    args := newReqMyLeafDoLeaf()
    if err := args.Read(iprot); err != nil {
        return nil, err
    }
    iprot.ReadMessageEnd()
    return args, nil
}

func (p *procFuncMyLeafDoLeaf) Write(seqId int32, result thrift.WritableStruct, oprot thrift.Protocol) (err thrift.Exception) {
    var err2 error
    messageType := thrift.REPLY
    if _, ok := result.(thrift.ApplicationException); ok {
        messageType = thrift.EXCEPTION
    }
    if err2 = oprot.WriteMessageBegin("DoLeaf", messageType, seqId); err2 != nil {
        err = err2
    }
    if err2 = result.Write(oprot); err == nil && err2 != nil {
        err = err2
    }
    if err2 = oprot.WriteMessageEnd(); err == nil && err2 != nil {
        err = err2
    }
    if err2 = oprot.Flush(); err == nil && err2 != nil {
        err = err2
    }
    return err
}

func (p *procFuncMyLeafDoLeaf) Run(reqStruct thrift.Struct) (thrift.WritableStruct, thrift.ApplicationException) {
    result := newRespMyLeafDoLeaf()
    err := p.handler.DoLeaf()
    if err != nil {
        x := thrift.NewApplicationExceptionCause(thrift.INTERNAL_ERROR, "Internal error processing DoLeaf: " + err.Error(), err)
        return x, x
    }

    return result, nil
}


