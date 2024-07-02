/**
 * Autogenerated by Thrift for hsmodule.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#include <thrift/compiler/test/fixtures/namespace/gen-py3/hsmodule/clients_wrapper.h>

namespace cpp2 {


folly::Future<int64_t>
HsTestServiceClientWrapper::init(
    apache::thrift::RpcOptions& rpcOptions,
    int64_t arg_int1) {
  auto* client = static_cast<::cpp2::HsTestServiceAsyncClient*>(async_client_.get());
  folly::Promise<int64_t> _promise;
  auto _future = _promise.getFuture();
  auto callback = std::make_unique<::thrift::py3::FutureCallback<int64_t>>(
    std::move(_promise), rpcOptions, client->recv_wrapped_init, channel_);
  try {
    client->init(
      rpcOptions,
      std::move(callback),
      arg_int1
    );
  } catch (...) {
    return folly::makeFuture<int64_t>(folly::exception_wrapper(
      std::current_exception()
    ));
  }
  return _future;
}

} // namespace cpp2
