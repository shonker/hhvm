/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/inheritance/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include "thrift/compiler/test/fixtures/inheritance/gen-cpp2/MyNode.h"

#include <thrift/lib/cpp2/gen/service_tcc.h>

namespace cpp2 {
typedef apache::thrift::ThriftPresult<false> MyNode_do_mid_pargs;
typedef apache::thrift::ThriftPresult<true> MyNode_do_mid_presult;
template <typename ProtocolIn_, typename ProtocolOut_>
void MyNodeAsyncProcessor::setUpAndProcess_do_mid(apache::thrift::ResponseChannelRequest::UniquePtr req, apache::thrift::SerializedCompressedRequest&& serializedRequest, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, [[maybe_unused]] apache::thrift::concurrency::ThreadManager* tm) {
  if (!setUpRequestProcessing(req, ctx, eb, tm, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, iface_)) {
    return;
  }
  auto scope = iface_->getRequestExecutionScope(ctx, apache::thrift::concurrency::NORMAL);
  ctx->setRequestExecutionScope(std::move(scope));
  processInThread(std::move(req), std::move(serializedRequest), ctx, eb, tm, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, &MyNodeAsyncProcessor::executeRequest_do_mid<ProtocolIn_, ProtocolOut_>, this);
}

template <typename ProtocolIn_, typename ProtocolOut_>
void MyNodeAsyncProcessor::executeRequest_do_mid(apache::thrift::ServerRequest&& serverRequest) {
  // make sure getRequestContext is null
  // so async calls don't accidentally use it
  iface_->setRequestContext(nullptr);
  struct ArgsState {
    ::cpp2::MyNode_do_mid_pargs pargs() {
      ::cpp2::MyNode_do_mid_pargs args;
      return args;
    }

    auto asTupleOfRefs() & {
      return std::tie(
      );
    }
  } args;

  auto ctxStack = apache::thrift::ContextStack::create(
    this->getEventHandlersSharedPtr(),
    this->getServiceName(),
    "MyNode.do_mid",
    serverRequest.requestContext());
  try {
    auto pargs = args.pargs();
    deserializeRequest<ProtocolIn_>(pargs, "do_mid", apache::thrift::detail::ServerRequestHelper::compressedRequest(std::move(serverRequest)).uncompress(), ctxStack.get());
  }
  catch (...) {
    folly::exception_wrapper ew(std::current_exception());
    apache::thrift::detail::ap::process_handle_exn_deserialization<ProtocolOut_>(
        ew
        , apache::thrift::detail::ServerRequestHelper::request(std::move(serverRequest))
        , serverRequest.requestContext()
        , apache::thrift::detail::ServerRequestHelper::eventBase(serverRequest)
        , "do_mid");
    return;
  }
  auto requestPileNotification = apache::thrift::detail::ServerRequestHelper::moveRequestPileNotification(serverRequest);
  auto concurrencyControllerNotification = apache::thrift::detail::ServerRequestHelper::moveConcurrencyControllerNotification(serverRequest);
  auto callback = apache::thrift::HandlerCallbackPtr<void>::make(
    apache::thrift::detail::ServerRequestHelper::request(std::move(serverRequest))
    , std::move(ctxStack)
    , return_do_mid<ProtocolIn_,ProtocolOut_>
    , throw_wrapped_do_mid<ProtocolIn_, ProtocolOut_>
    , serverRequest.requestContext()->getProtoSeqId()
    , apache::thrift::detail::ServerRequestHelper::eventBase(serverRequest)
    , apache::thrift::detail::ServerRequestHelper::executor(serverRequest)
    , serverRequest.requestContext()
    , requestPileNotification
    , concurrencyControllerNotification, std::move(serverRequest.requestData())
    );
  const auto makeExecuteHandler = [&] {
    return [ifacePtr = iface_](auto&& cb, ArgsState args) mutable {
      (void)args;
      ifacePtr->async_tm_do_mid(std::move(cb));
    };
  };
#if FOLLY_HAS_COROUTINES
  if (apache::thrift::detail::shouldProcessServiceInterceptorsOnRequest(*callback)) {
    [](auto callback, auto executeHandler, ArgsState args) -> folly::coro::Task<void> {
      auto argRefs = args.asTupleOfRefs();
      co_await apache::thrift::detail::processServiceInterceptorsOnRequest(
          *callback,
          apache::thrift::detail::ServiceInterceptorOnRequestArguments(argRefs));
      executeHandler(std::move(callback), std::move(args));
    }(std::move(callback), makeExecuteHandler(), std::move(args))
              .scheduleOn(apache::thrift::detail::ServerRequestHelper::executor(serverRequest))
              .startInlineUnsafe();
  } else {
    makeExecuteHandler()(std::move(callback), std::move(args));
  }
#else
  makeExecuteHandler()(std::move(callback), std::move(args));
#endif // FOLLY_HAS_COROUTINES
}

template <class ProtocolIn_, class ProtocolOut_>
apache::thrift::SerializedResponse MyNodeAsyncProcessor::return_do_mid(apache::thrift::ContextStack* ctx) {
  ProtocolOut_ prot;
  ::cpp2::MyNode_do_mid_presult result;
  return serializeResponse("do_mid", &prot, ctx, result);
}

template <class ProtocolIn_, class ProtocolOut_>
void MyNodeAsyncProcessor::throw_wrapped_do_mid(apache::thrift::ResponseChannelRequest::UniquePtr req,[[maybe_unused]] int32_t protoSeqId,apache::thrift::ContextStack* ctx,folly::exception_wrapper ew,apache::thrift::Cpp2RequestContext* reqCtx) {
  if (!ew) {
    return;
  }
  {
    apache::thrift::detail::ap::process_throw_wrapped_handler_error<ProtocolOut_>(
        ew, std::move(req), reqCtx, ctx, "do_mid");
    return;
  }
}


} // namespace cpp2
