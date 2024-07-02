/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/crypto/Crypto.h>
#include <folly/io/IOBuf.h>

namespace fizz {
/**
 * An HKDF implementation conformant with
 * https://tools.ietf.org/html/rfc5869.
 *
 * outputBytes represents the number of bytes
 * required as output from the HKDF. Other
 * params are as defined by the RFC.
 */
class Hkdf {
 public:
  virtual ~Hkdf() = default;

  virtual std::vector<uint8_t> extract(
      folly::ByteRange salt,
      folly::ByteRange ikm) const = 0;

  virtual std::unique_ptr<folly::IOBuf> expand(
      folly::ByteRange extractedKey,
      const folly::IOBuf& info,
      size_t outputBytes) const = 0;

  virtual std::unique_ptr<folly::IOBuf> hkdf(
      folly::ByteRange ikm,
      folly::ByteRange salt,
      const folly::IOBuf& info,
      size_t outputBytes) const = 0;

  virtual size_t hashLength() const = 0;
};

/**
 * HKDF implementation using a templated HMAC implementation.
 *
 * The template struct requires the following parameters:
 *   - HashLen: length of the hash digest
 *   - hmac(ByteRange key, const IOBuf& in, MutableByteRange out)
 */
class HkdfImpl : public Hkdf {
 public:
  HkdfImpl(size_t hashLength, HmacFunc hmacFunc)
      : hashLength_(hashLength), hmacFunc_(hmacFunc) {}

  std::vector<uint8_t> extract(folly::ByteRange salt, folly::ByteRange ikm)
      const override;

  std::unique_ptr<folly::IOBuf> expand(
      folly::ByteRange extractedKey,
      const folly::IOBuf& info,
      size_t outputBytes) const override;

  std::unique_ptr<folly::IOBuf> hkdf(
      folly::ByteRange ikm,
      folly::ByteRange salt,
      const folly::IOBuf& info,
      size_t outputBytes) const override;

  size_t hashLength() const override {
    return hashLength_;
  }

 private:
  size_t hashLength_;
  HmacFunc hmacFunc_;
};
} // namespace fizz
