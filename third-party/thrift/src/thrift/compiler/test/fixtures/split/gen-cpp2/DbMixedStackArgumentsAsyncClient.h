/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/split/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include <thrift/lib/cpp2/gen/client_h.h>

#include "thrift/compiler/test/fixtures/split/gen-cpp2/module_types.h"

namespace apache { namespace thrift {
  class Cpp2RequestContext;
  namespace detail { namespace ac { struct ClientRequestContext; }}
  namespace transport { class THeader; }
}}

namespace cpp2 {
class DbMixedStackArguments;
} // cpp2
namespace apache::thrift {

template <>
class Client<::cpp2::DbMixedStackArguments> : public apache::thrift::GeneratedAsyncClient {
 public:
  using apache::thrift::GeneratedAsyncClient::GeneratedAsyncClient;

  char const* getServiceName() const noexcept override {
    return "DbMixedStackArguments";
  }


  virtual void getDataByKey0(std::unique_ptr<apache::thrift::RequestCallback> callback, const ::std::string& p_key);
  virtual void getDataByKey0(apache::thrift::RpcOptions& rpcOptions, std::unique_ptr<apache::thrift::RequestCallback> callback, const ::std::string& p_key);
 protected:
  void getDataByKey0Impl(apache::thrift::RpcOptions& rpcOptions, std::shared_ptr<apache::thrift::transport::THeader> header, apache::thrift::ContextStack* contextStack, apache::thrift::RequestClientCallback::Ptr callback, const ::std::string& p_key, bool stealRpcOptions = false);
 public:

  virtual void sync_getDataByKey0(::std::string& _return, const ::std::string& p_key);
  virtual void sync_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, ::std::string& _return, const ::std::string& p_key);

  virtual folly::Future<::std::string> future_getDataByKey0(const ::std::string& p_key);
  virtual folly::SemiFuture<::std::string> semifuture_getDataByKey0(const ::std::string& p_key);
  virtual folly::Future<::std::string> future_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);
  virtual folly::SemiFuture<::std::string> semifuture_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);
  virtual folly::Future<std::pair<::std::string, std::unique_ptr<apache::thrift::transport::THeader>>> header_future_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);
  virtual folly::SemiFuture<std::pair<::std::string, std::unique_ptr<apache::thrift::transport::THeader>>> header_semifuture_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);

#if FOLLY_HAS_COROUTINES
#if __clang__
  template <int = 0>
  folly::coro::Task<::std::string> co_getDataByKey0(const ::std::string& p_key) {
    return co_getDataByKey0<false>(nullptr, p_key);
  }
  template <int = 0>
  folly::coro::Task<::std::string> co_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key) {
    return co_getDataByKey0<true>(&rpcOptions, p_key);
  }
#else
  folly::coro::Task<::std::string> co_getDataByKey0(const ::std::string& p_key) {
    co_return co_await folly::coro::detachOnCancel(semifuture_getDataByKey0(p_key));
  }
  folly::coro::Task<::std::string> co_getDataByKey0(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key) {
    co_return co_await folly::coro::detachOnCancel(semifuture_getDataByKey0(rpcOptions, p_key));
  }
#endif
 private:
  template <bool hasRpcOptions>
  folly::coro::Task<::std::string> co_getDataByKey0(apache::thrift::RpcOptions* rpcOptions, const ::std::string& p_key) {
    const folly::CancellationToken& cancelToken =
        co_await folly::coro::co_current_cancellation_token;
    const bool cancellable = cancelToken.canBeCancelled();
    apache::thrift::ClientReceiveState returnState;
    apache::thrift::ClientCoroCallback<false> callback(&returnState, co_await folly::coro::co_current_executor);
    auto protocolId = apache::thrift::GeneratedAsyncClient::getChannel()->getProtocolId();
    auto [ctx, header] = getDataByKey0Ctx(rpcOptions);
    using CancellableCallback = apache::thrift::CancellableRequestClientCallback<false>;
    auto cancellableCallback = cancellable ? CancellableCallback::create(&callback, channel_) : nullptr;
    static apache::thrift::RpcOptions defaultRpcOptions;
    auto wrappedCallback = apache::thrift::RequestClientCallback::Ptr(cancellableCallback ? (apache::thrift::RequestClientCallback*)cancellableCallback.get() : &callback);
    if constexpr (hasRpcOptions) {
      getDataByKey0Impl(*rpcOptions, std::move(header), ctx.get(), std::move(wrappedCallback), p_key);
    } else {
      getDataByKey0Impl(defaultRpcOptions, std::move(header), ctx.get(), std::move(wrappedCallback), p_key);
    }
    if (cancellable) {
      folly::CancellationCallback cb(cancelToken, [&] { CancellableCallback::cancel(std::move(cancellableCallback)); });
      co_await callback.co_waitUntilDone();
    } else {
      co_await callback.co_waitUntilDone();
    }
    if (returnState.isException()) {
      co_yield folly::coro::co_error(std::move(returnState.exception()));
    }
    returnState.resetProtocolId(protocolId);
    returnState.resetCtx(std::move(ctx));
    SCOPE_EXIT {
      if (hasRpcOptions && returnState.header()) {
        auto* rheader = returnState.header();
        if (!rheader->getHeaders().empty()) {
          rpcOptions->setReadHeaders(rheader->releaseHeaders());
        }
        rpcOptions->setRoutingData(rheader->releaseRoutingData());
      }
    };
    ::std::string _return;
    if (auto ew = recv_wrapped_getDataByKey0(_return, returnState)) {
      co_yield folly::coro::co_error(std::move(ew));
    }
    co_return _return;
  }
 public:
#endif // FOLLY_HAS_COROUTINES

  virtual void getDataByKey0(folly::Function<void (::apache::thrift::ClientReceiveState&&)> callback, const ::std::string& p_key);


  static folly::exception_wrapper recv_wrapped_getDataByKey0(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
  static void recv_getDataByKey0(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
  // Mock friendly virtual instance method
  virtual void recv_instance_getDataByKey0(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
  virtual folly::exception_wrapper recv_instance_wrapped_getDataByKey0(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
 private:
  template <typename Protocol_, typename RpcOptions>
  void getDataByKey0T(Protocol_* prot, RpcOptions&& rpcOptions, std::shared_ptr<apache::thrift::transport::THeader> header, apache::thrift::ContextStack* contextStack, apache::thrift::RequestClientCallback::Ptr callback, const ::std::string& p_key);
  std::pair<::apache::thrift::ContextStack::UniquePtr, std::shared_ptr<::apache::thrift::transport::THeader>> getDataByKey0Ctx(apache::thrift::RpcOptions* rpcOptions);
 public:
  virtual void getDataByKey1(std::unique_ptr<apache::thrift::RequestCallback> callback, const ::std::string& p_key);
  virtual void getDataByKey1(apache::thrift::RpcOptions& rpcOptions, std::unique_ptr<apache::thrift::RequestCallback> callback, const ::std::string& p_key);
 protected:
  void getDataByKey1Impl(apache::thrift::RpcOptions& rpcOptions, std::shared_ptr<apache::thrift::transport::THeader> header, apache::thrift::ContextStack* contextStack, apache::thrift::RequestClientCallback::Ptr callback, const ::std::string& p_key, bool stealRpcOptions = false);
 public:

  virtual void sync_getDataByKey1(::std::string& _return, const ::std::string& p_key);
  virtual void sync_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, ::std::string& _return, const ::std::string& p_key);

  virtual folly::Future<::std::string> future_getDataByKey1(const ::std::string& p_key);
  virtual folly::SemiFuture<::std::string> semifuture_getDataByKey1(const ::std::string& p_key);
  virtual folly::Future<::std::string> future_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);
  virtual folly::SemiFuture<::std::string> semifuture_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);
  virtual folly::Future<std::pair<::std::string, std::unique_ptr<apache::thrift::transport::THeader>>> header_future_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);
  virtual folly::SemiFuture<std::pair<::std::string, std::unique_ptr<apache::thrift::transport::THeader>>> header_semifuture_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key);

#if FOLLY_HAS_COROUTINES
#if __clang__
  template <int = 0>
  folly::coro::Task<::std::string> co_getDataByKey1(const ::std::string& p_key) {
    return co_getDataByKey1<false>(nullptr, p_key);
  }
  template <int = 0>
  folly::coro::Task<::std::string> co_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key) {
    return co_getDataByKey1<true>(&rpcOptions, p_key);
  }
#else
  folly::coro::Task<::std::string> co_getDataByKey1(const ::std::string& p_key) {
    co_return co_await folly::coro::detachOnCancel(semifuture_getDataByKey1(p_key));
  }
  folly::coro::Task<::std::string> co_getDataByKey1(apache::thrift::RpcOptions& rpcOptions, const ::std::string& p_key) {
    co_return co_await folly::coro::detachOnCancel(semifuture_getDataByKey1(rpcOptions, p_key));
  }
#endif
 private:
  template <bool hasRpcOptions>
  folly::coro::Task<::std::string> co_getDataByKey1(apache::thrift::RpcOptions* rpcOptions, const ::std::string& p_key) {
    const folly::CancellationToken& cancelToken =
        co_await folly::coro::co_current_cancellation_token;
    const bool cancellable = cancelToken.canBeCancelled();
    apache::thrift::ClientReceiveState returnState;
    apache::thrift::ClientCoroCallback<false> callback(&returnState, co_await folly::coro::co_current_executor);
    auto protocolId = apache::thrift::GeneratedAsyncClient::getChannel()->getProtocolId();
    auto [ctx, header] = getDataByKey1Ctx(rpcOptions);
    using CancellableCallback = apache::thrift::CancellableRequestClientCallback<false>;
    auto cancellableCallback = cancellable ? CancellableCallback::create(&callback, channel_) : nullptr;
    static apache::thrift::RpcOptions defaultRpcOptions;
    auto wrappedCallback = apache::thrift::RequestClientCallback::Ptr(cancellableCallback ? (apache::thrift::RequestClientCallback*)cancellableCallback.get() : &callback);
    if constexpr (hasRpcOptions) {
      getDataByKey1Impl(*rpcOptions, std::move(header), ctx.get(), std::move(wrappedCallback), p_key);
    } else {
      getDataByKey1Impl(defaultRpcOptions, std::move(header), ctx.get(), std::move(wrappedCallback), p_key);
    }
    if (cancellable) {
      folly::CancellationCallback cb(cancelToken, [&] { CancellableCallback::cancel(std::move(cancellableCallback)); });
      co_await callback.co_waitUntilDone();
    } else {
      co_await callback.co_waitUntilDone();
    }
    if (returnState.isException()) {
      co_yield folly::coro::co_error(std::move(returnState.exception()));
    }
    returnState.resetProtocolId(protocolId);
    returnState.resetCtx(std::move(ctx));
    SCOPE_EXIT {
      if (hasRpcOptions && returnState.header()) {
        auto* rheader = returnState.header();
        if (!rheader->getHeaders().empty()) {
          rpcOptions->setReadHeaders(rheader->releaseHeaders());
        }
        rpcOptions->setRoutingData(rheader->releaseRoutingData());
      }
    };
    ::std::string _return;
    if (auto ew = recv_wrapped_getDataByKey1(_return, returnState)) {
      co_yield folly::coro::co_error(std::move(ew));
    }
    co_return _return;
  }
 public:
#endif // FOLLY_HAS_COROUTINES

  virtual void getDataByKey1(folly::Function<void (::apache::thrift::ClientReceiveState&&)> callback, const ::std::string& p_key);


  static folly::exception_wrapper recv_wrapped_getDataByKey1(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
  static void recv_getDataByKey1(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
  // Mock friendly virtual instance method
  virtual void recv_instance_getDataByKey1(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
  virtual folly::exception_wrapper recv_instance_wrapped_getDataByKey1(::std::string& _return, ::apache::thrift::ClientReceiveState& state);
 private:
  template <typename Protocol_, typename RpcOptions>
  void getDataByKey1T(Protocol_* prot, RpcOptions&& rpcOptions, std::shared_ptr<apache::thrift::transport::THeader> header, apache::thrift::ContextStack* contextStack, apache::thrift::RequestClientCallback::Ptr callback, const ::std::string& p_key);
  std::pair<::apache::thrift::ContextStack::UniquePtr, std::shared_ptr<::apache::thrift::transport::THeader>> getDataByKey1Ctx(apache::thrift::RpcOptions* rpcOptions);
 public:
};

} // namespace apache::thrift

namespace cpp2 {
using DbMixedStackArgumentsAsyncClient [[deprecated("Use apache::thrift::Client<DbMixedStackArguments> instead")]] = ::apache::thrift::Client<DbMixedStackArguments>;
} // cpp2
