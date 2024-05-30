/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/adapter/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include "thrift/compiler/test/fixtures/adapter/gen-cpp2/Service.h"

#include <thrift/lib/cpp2/gen/service_tcc.h>

namespace facebook::thrift::test {
typedef apache::thrift::ThriftPresult<false, apache::thrift::FieldData<1, ::apache::thrift::type_class::string, ::facebook::thrift::test::StringWithAdapter_7208*, ::apache::thrift::type::adapted<::my::Adapter1, ::apache::thrift::type::string_t>>, apache::thrift::FieldData<2, ::apache::thrift::type_class::string, ::std::string*>, apache::thrift::FieldData<3, ::apache::thrift::type_class::structure, ::facebook::thrift::test::Foo*>> Service_func_pargs;
typedef apache::thrift::ThriftPresult<true, apache::thrift::FieldData<0, ::apache::thrift::type_class::integral, ::facebook::thrift::test::MyI32_4873*, ::apache::thrift::type::adapted<::my::Adapter1, ::apache::thrift::type::i32_t>>> Service_func_presult;
template <typename ProtocolIn_, typename ProtocolOut_>
void ServiceAsyncProcessor::setUpAndProcess_func(apache::thrift::ResponseChannelRequest::UniquePtr req, apache::thrift::SerializedCompressedRequest&& serializedRequest, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, [[maybe_unused]] apache::thrift::concurrency::ThreadManager* tm) {
  if (!setUpRequestProcessing(req, ctx, eb, tm, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, iface_)) {
    return;
  }
  auto scope = iface_->getRequestExecutionScope(ctx, apache::thrift::concurrency::NORMAL);
  ctx->setRequestExecutionScope(std::move(scope));
  processInThread(std::move(req), std::move(serializedRequest), ctx, eb, tm, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, &ServiceAsyncProcessor::executeRequest_func<ProtocolIn_, ProtocolOut_>, this);
}

template <typename ProtocolIn_, typename ProtocolOut_>
void ServiceAsyncProcessor::executeRequest_func(apache::thrift::ServerRequest&& serverRequest) {
  // make sure getRequestContext is null
  // so async calls don't accidentally use it
  iface_->setRequestContext(nullptr);
  struct ArgsState {
    std::unique_ptr<::facebook::thrift::test::StringWithAdapter_7208> uarg_arg1 = std::make_unique<::facebook::thrift::test::StringWithAdapter_7208>();
    std::unique_ptr<::std::string> uarg_arg2 = std::make_unique<::std::string>();
    std::unique_ptr<::facebook::thrift::test::Foo> uarg_arg3 = std::make_unique<::facebook::thrift::test::Foo>();
    ::facebook::thrift::test::Service_func_pargs pargs() {
      ::facebook::thrift::test::Service_func_pargs args;
      args.get<0>().value = uarg_arg1.get();
      args.get<1>().value = uarg_arg2.get();
      args.get<2>().value = uarg_arg3.get();
      return args;
    }
  } args;

  auto ctxStack = apache::thrift::ContextStack::create(
    this->getEventHandlersSharedPtr(),
    this->getServiceName(),
    "Service.func",
    serverRequest.requestContext());
  try {
    auto pargs = args.pargs();
    deserializeRequest<ProtocolIn_>(pargs, "func", apache::thrift::detail::ServerRequestHelper::compressedRequest(std::move(serverRequest)).uncompress(), ctxStack.get());
  }
  catch (...) {
    folly::exception_wrapper ew(std::current_exception());
    apache::thrift::detail::ap::process_handle_exn_deserialization<ProtocolOut_>(
        ew
        , apache::thrift::detail::ServerRequestHelper::request(std::move(serverRequest))
        , serverRequest.requestContext()
        , apache::thrift::detail::ServerRequestHelper::eventBase(serverRequest)
        , "func");
    return;
  }
  auto requestPileNotification = apache::thrift::detail::ServerRequestHelper::moveRequestPileNotification(serverRequest);
  auto concurrencyControllerNotification = apache::thrift::detail::ServerRequestHelper::moveConcurrencyControllerNotification(serverRequest);
  auto callback = apache::thrift::HandlerCallbackPtr<::facebook::thrift::test::MyI32_4873>::make(
    apache::thrift::detail::ServerRequestHelper::request(std::move(serverRequest))
    , std::move(ctxStack)
    , return_func<ProtocolIn_,ProtocolOut_>
    , throw_wrapped_func<ProtocolIn_, ProtocolOut_>
    , serverRequest.requestContext()->getProtoSeqId()
    , apache::thrift::detail::ServerRequestHelper::eventBase(serverRequest)
    , apache::thrift::detail::ServerRequestHelper::executor(serverRequest)
    , serverRequest.requestContext()
    , requestPileNotification
    , concurrencyControllerNotification, std::move(serverRequest.requestData())
    );
  const auto makeExecuteHandler = [&] {
    return [ifacePtr = iface_, args = std::move(args)](auto&& cb) mutable {
      (void)args;
      ifacePtr->async_tm_func(std::move(cb), std::move(args.uarg_arg1), std::move(args.uarg_arg2), std::move(args.uarg_arg3));
    };
  };
#if FOLLY_HAS_COROUTINES
  if (apache::thrift::detail::shouldProcessServiceInterceptorsOnRequest(*callback)) {
    [](auto callback, auto executeHandler) -> folly::coro::Task<void> {
      co_await apache::thrift::detail::processServiceInterceptorsOnRequest(*callback);
      executeHandler(std::move(callback));
    }(std::move(callback), makeExecuteHandler())
              .scheduleOn(apache::thrift::detail::ServerRequestHelper::executor(serverRequest))
              .startInlineUnsafe();
  } else {
    makeExecuteHandler()(std::move(callback));
  }
#else
  makeExecuteHandler()(std::move(callback));
#endif // FOLLY_HAS_COROUTINES
}

template <class ProtocolIn_, class ProtocolOut_>
apache::thrift::SerializedResponse ServiceAsyncProcessor::return_func(apache::thrift::ContextStack* ctx, ::facebook::thrift::test::MyI32_4873 const& _return) {
  ProtocolOut_ prot;
  ::facebook::thrift::test::Service_func_presult result;
  result.get<0>().value = const_cast<::facebook::thrift::test::MyI32_4873*>(&_return);
  result.setIsSet(0, true);
  return serializeResponse("func", &prot, ctx, result);
}

template <class ProtocolIn_, class ProtocolOut_>
void ServiceAsyncProcessor::throw_wrapped_func(apache::thrift::ResponseChannelRequest::UniquePtr req,[[maybe_unused]] int32_t protoSeqId,apache::thrift::ContextStack* ctx,folly::exception_wrapper ew,apache::thrift::Cpp2RequestContext* reqCtx) {
  if (!ew) {
    return;
  }
  {
    apache::thrift::detail::ap::process_throw_wrapped_handler_error<ProtocolOut_>(
        ew, std::move(req), reqCtx, ctx, "func");
    return;
  }
}


} // namespace facebook::thrift::test
