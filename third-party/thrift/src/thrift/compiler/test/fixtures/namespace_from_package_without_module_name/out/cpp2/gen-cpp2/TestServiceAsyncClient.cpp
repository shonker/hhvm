/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/namespace_from_package_without_module_name/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */

#include "thrift/compiler/test/fixtures/namespace_from_package_without_module_name/gen-cpp2/TestServiceAsyncClient.h"

#include <thrift/lib/cpp2/gen/client_cpp.h>

namespace test::namespace_from_package_without_module_name {
typedef apache::thrift::ThriftPresult<false, apache::thrift::FieldData<1, ::apache::thrift::type_class::integral, ::std::int64_t*>> TestService_init_pargs;
typedef apache::thrift::ThriftPresult<true, apache::thrift::FieldData<0, ::apache::thrift::type_class::integral, ::std::int64_t*>> TestService_init_presult;
} // namespace test::namespace_from_package_without_module_name
template <typename Protocol_, typename RpcOptions>
void apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::initT(Protocol_* prot, RpcOptions&& rpcOptions, std::shared_ptr<apache::thrift::transport::THeader> header, apache::thrift::ContextStack* contextStack, apache::thrift::RequestClientCallback::Ptr callback, ::std::int64_t p_int1) {

  ::test::namespace_from_package_without_module_name::TestService_init_pargs args;
  args.get<0>().value = &p_int1;
  auto sizer = [&](Protocol_* p) { return args.serializedSizeZC(p); };
  auto writer = [&](Protocol_* p) { args.write(p); };

  static ::apache::thrift::MethodMetadata::Data* methodMetadata =
        new ::apache::thrift::MethodMetadata::Data(
                "init",
                ::apache::thrift::FunctionQualifier::Unspecified,
                "test.dev/namespace_from_package_without_module_name/TestService");
  apache::thrift::clientSendT<apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, Protocol_>(prot, std::forward<RpcOptions>(rpcOptions), std::move(callback), contextStack, std::move(header), channel_.get(), ::apache::thrift::MethodMetadata::from_static(methodMetadata), writer, sizer);
}



void apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::init(std::unique_ptr<apache::thrift::RequestCallback> callback, ::std::int64_t p_int1) {
  ::apache::thrift::RpcOptions rpcOptions;
  init(rpcOptions, std::move(callback), p_int1);
}

void apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::init(apache::thrift::RpcOptions& rpcOptions, std::unique_ptr<apache::thrift::RequestCallback> callback, ::std::int64_t p_int1) {
  auto [ctx, header] = initCtx(&rpcOptions);
  apache::thrift::RequestCallback::Context callbackContext;
  callbackContext.protocolId =
      apache::thrift::GeneratedAsyncClient::getChannel()->getProtocolId();
  auto* contextStack = ctx.get();
  if (callback) {
    callbackContext.ctx = std::move(ctx);
  }
  auto wrappedCallback = apache::thrift::toRequestClientCallbackPtr(std::move(callback), std::move(callbackContext));
  initImpl(rpcOptions, std::move(header), contextStack, std::move(wrappedCallback), p_int1);
}

void apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::initImpl(apache::thrift::RpcOptions& rpcOptions, std::shared_ptr<apache::thrift::transport::THeader> header, apache::thrift::ContextStack* contextStack, apache::thrift::RequestClientCallback::Ptr callback, ::std::int64_t p_int1, bool stealRpcOptions) {
  switch (apache::thrift::GeneratedAsyncClient::getChannel()->getProtocolId()) {
    case apache::thrift::protocol::T_BINARY_PROTOCOL:
    {
      apache::thrift::BinaryProtocolWriter writer;
      if (stealRpcOptions) {
        initT(&writer, std::move(rpcOptions), std::move(header), contextStack, std::move(callback), p_int1);
      } else {
        initT(&writer, rpcOptions, std::move(header), contextStack, std::move(callback), p_int1);
      }
      break;
    }
    case apache::thrift::protocol::T_COMPACT_PROTOCOL:
    {
      apache::thrift::CompactProtocolWriter writer;
      if (stealRpcOptions) {
        initT(&writer, std::move(rpcOptions), std::move(header), contextStack, std::move(callback), p_int1);
      } else {
        initT(&writer, rpcOptions, std::move(header), contextStack, std::move(callback), p_int1);
      }
      break;
    }
    default:
    {
      apache::thrift::detail::ac::throw_app_exn("Could not find Protocol");
    }
  }
}

std::pair<::apache::thrift::ContextStack::UniquePtr, std::shared_ptr<::apache::thrift::transport::THeader>> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::initCtx(apache::thrift::RpcOptions* rpcOptions) {
  auto header = std::make_shared<apache::thrift::transport::THeader>(
      apache::thrift::transport::THeader::ALLOW_BIG_FRAMES);
  header->setProtocolId(channel_->getProtocolId());
  if (rpcOptions) {
    header->setHeaders(rpcOptions->releaseWriteHeaders());
  }

  auto ctx = apache::thrift::ContextStack::createWithClientContext(
      handlers_,
      getServiceName(),
      "TestService.init",
      *header);

  return {std::move(ctx), std::move(header)};
}

::std::int64_t apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::sync_init(::std::int64_t p_int1) {
  ::apache::thrift::RpcOptions rpcOptions;
  return sync_init(rpcOptions, p_int1);
}

::std::int64_t apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::sync_init(apache::thrift::RpcOptions& rpcOptions, ::std::int64_t p_int1) {
  apache::thrift::ClientReceiveState returnState;
  apache::thrift::ClientSyncCallback<false> callback(&returnState);
  auto protocolId = apache::thrift::GeneratedAsyncClient::getChannel()->getProtocolId();
  auto evb = apache::thrift::GeneratedAsyncClient::getChannel()->getEventBase();
  auto ctxAndHeader = initCtx(&rpcOptions);
  auto wrappedCallback = apache::thrift::RequestClientCallback::Ptr(&callback);
  callback.waitUntilDone(
    evb,
    [&] {
      initImpl(rpcOptions, std::move(ctxAndHeader.second), ctxAndHeader.first.get(), std::move(wrappedCallback), p_int1);
    });

  if (returnState.isException()) {
    returnState.exception().throw_exception();
  }
  returnState.resetProtocolId(protocolId);
  returnState.resetCtx(std::move(ctxAndHeader.first));
  SCOPE_EXIT {
    if (returnState.header() && !returnState.header()->getHeaders().empty()) {
      rpcOptions.setReadHeaders(returnState.header()->releaseHeaders());
    }
  };
  return folly::fibers::runInMainContext([&] {
      return recv_init(returnState);
  });
}


folly::Future<::std::int64_t> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::future_init(::std::int64_t p_int1) {
  ::apache::thrift::RpcOptions rpcOptions;
  return future_init(rpcOptions, p_int1);
}

folly::SemiFuture<::std::int64_t> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::semifuture_init(::std::int64_t p_int1) {
  ::apache::thrift::RpcOptions rpcOptions;
  return semifuture_init(rpcOptions, p_int1);
}

folly::Future<::std::int64_t> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::future_init(apache::thrift::RpcOptions& rpcOptions, ::std::int64_t p_int1) {
  folly::Promise<::std::int64_t> promise;
  auto future = promise.getFuture();
  auto callback = std::make_unique<apache::thrift::FutureCallback<::std::int64_t>>(std::move(promise), recv_wrapped_init, channel_);
  init(rpcOptions, std::move(callback), p_int1);
  return future;
}

folly::SemiFuture<::std::int64_t> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::semifuture_init(apache::thrift::RpcOptions& rpcOptions, ::std::int64_t p_int1) {
  auto callbackAndFuture = makeSemiFutureCallback(recv_wrapped_init, channel_);
  auto callback = std::move(callbackAndFuture.first);
  init(rpcOptions, std::move(callback), p_int1);
  return std::move(callbackAndFuture.second);
}

folly::Future<std::pair<::std::int64_t, std::unique_ptr<apache::thrift::transport::THeader>>> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::header_future_init(apache::thrift::RpcOptions& rpcOptions, ::std::int64_t p_int1) {
  folly::Promise<std::pair<::std::int64_t, std::unique_ptr<apache::thrift::transport::THeader>>> promise;
  auto future = promise.getFuture();
  auto callback = std::make_unique<apache::thrift::HeaderFutureCallback<::std::int64_t>>(std::move(promise), recv_wrapped_init, channel_);
  init(rpcOptions, std::move(callback), p_int1);
  return future;
}

folly::SemiFuture<std::pair<::std::int64_t, std::unique_ptr<apache::thrift::transport::THeader>>> apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::header_semifuture_init(apache::thrift::RpcOptions& rpcOptions, ::std::int64_t p_int1) {
  auto callbackAndFuture = makeHeaderSemiFutureCallback(recv_wrapped_init, channel_);
  auto callback = std::move(callbackAndFuture.first);
  init(rpcOptions, std::move(callback), p_int1);
  return std::move(callbackAndFuture.second);
}

void apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::init(folly::Function<void (::apache::thrift::ClientReceiveState&&)> callback, ::std::int64_t p_int1) {
  init(std::make_unique<apache::thrift::FunctionReplyCallback>(std::move(callback)), p_int1);
}

#if FOLLY_HAS_COROUTINES
#endif // FOLLY_HAS_COROUTINES
folly::exception_wrapper apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::recv_wrapped_init(::std::int64_t& _return, ::apache::thrift::ClientReceiveState& state) {
  if (state.isException()) {
    return std::move(state.exception());
  }
  if (!state.hasResponseBuffer()) {
    return folly::make_exception_wrapper<apache::thrift::TApplicationException>("recv_ called without result");
  }

  using result = ::test::namespace_from_package_without_module_name::TestService_init_presult;
  switch (state.protocolId()) {
    case apache::thrift::protocol::T_BINARY_PROTOCOL:
    {
      apache::thrift::BinaryProtocolReader reader;
      return apache::thrift::detail::ac::recv_wrapped<result>(
          &reader, state, _return);
    }
    case apache::thrift::protocol::T_COMPACT_PROTOCOL:
    {
      apache::thrift::CompactProtocolReader reader;
      return apache::thrift::detail::ac::recv_wrapped<result>(
          &reader, state, _return);
    }
    default:
    {
    }
  }
  return folly::make_exception_wrapper<apache::thrift::TApplicationException>("Could not find Protocol");
}

::std::int64_t apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::recv_init(::apache::thrift::ClientReceiveState& state) {
  ::std::int64_t _return;
  auto ew = recv_wrapped_init(_return, state);
  if (ew) {
    ew.throw_exception();
  }
  return _return;
}

::std::int64_t apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::recv_instance_init(::apache::thrift::ClientReceiveState& state) {
  return recv_init(state);
}

folly::exception_wrapper apache::thrift::Client<::test::namespace_from_package_without_module_name::TestService>::recv_instance_wrapped_init(::std::int64_t& _return, ::apache::thrift::ClientReceiveState& state) {
  return recv_wrapped_init(_return, state);
}


