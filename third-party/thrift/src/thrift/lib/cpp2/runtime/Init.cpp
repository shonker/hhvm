/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <thrift/lib/cpp2/runtime/Init.h>

#include <stdexcept>

#include <folly/synchronization/DelayedInit.h>

namespace apache::thrift::runtime {

namespace {
struct RuntimeState {
  std::vector<std::shared_ptr<apache::thrift::TProcessorEventHandler>>
      legacyClientEventHandlers;
};
folly::DelayedInit<RuntimeState> gRuntimeState;
} // namespace

void init(InitOptions options) {
  bool didInitialize = false;
  gRuntimeState.try_emplace_with([&] {
    didInitialize = true;
    return RuntimeState{std::move(options.legacyClientEventHandlers)};
  });
  if (!didInitialize) {
    throw std::logic_error(
        "apache::thrift::runtime::init() was already called!");
  }
}

bool wasInitialized() noexcept {
  return gRuntimeState.has_value();
}

folly::Range<std::shared_ptr<apache::thrift::TProcessorEventHandler>*>
getGlobalLegacyClientEventHandlers() {
  if (!wasInitialized()) {
    return {};
  }
  return folly::range(gRuntimeState->legacyClientEventHandlers);
}

} // namespace apache::thrift::runtime
