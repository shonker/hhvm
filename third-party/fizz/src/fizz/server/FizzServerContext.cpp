/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include "fizz/server/FizzServerContext.h"

#include <fizz/backend/openssl/OpenSSLFactory.h>

namespace fizz {
namespace server {

FizzServerContext::FizzServerContext()
    : factory_(std::make_shared<openssl::OpenSSLFactory>()) {}

} // namespace server
} // namespace fizz
