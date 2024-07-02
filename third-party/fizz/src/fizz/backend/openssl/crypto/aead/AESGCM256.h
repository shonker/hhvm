/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <openssl/evp.h>

#include <fizz/backend/openssl/Properties.h>
#include <fizz/crypto/Crypto.h>

namespace fizz {
namespace openssl {

struct AESGCM256 {
  static constexpr auto Cipher = EVP_aes_256_gcm;

  static const size_t kKeyLength{32};
  static const size_t kIVLength{12};
  static const size_t kTagLength{16};
  static const bool kOperatesInBlocks{false};
  static const bool kRequiresPresetTagLen{false};
};

template <>
struct Properties<fizz::AESGCM256> {
  static const EVP_CIPHER* Cipher() {
    return EVP_aes_256_gcm();
  }
  static const bool kOperatesInBlocks{false};
  static const bool kRequiresPresetTagLen{false};
};

} // namespace openssl
} // namespace fizz
