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

#pragma once

#include <thrift/lib/cpp/TProcessorEventHandler.h>
#include <thrift/lib/cpp/server/TServerEventHandler.h>
#include <thrift/lib/cpp2/server/ServiceInterceptorBase.h>

namespace apache::thrift {

class ServerModule {
 public:
  virtual ~ServerModule() = default;

  virtual std::string getName() const = 0;

  virtual std::vector<std::shared_ptr<ServiceInterceptorBase>>
  getServiceInterceptors() {
    return {};
  }

  virtual std::vector<std::shared_ptr<TProcessorEventHandler>>
  getLegacyEventHandlers() {
    return {};
  }

  virtual std::vector<std::shared_ptr<server::TServerEventHandler>>
  getLegacyServerEventHandlers() {
    return {};
  }
};

} // namespace apache::thrift
