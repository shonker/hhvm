/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/backend/openssl/crypto/signature/Signature.h>
#include <fizz/protocol/Certificate.h>
#include <folly/io/async/ssl/OpenSSLTransportCertificate.h>

namespace fizz {
class PeerCert;
enum class CertificateVerifyContext;

namespace openssl {

template <KeyType T>
class OpenSSLPeerCertImpl : public fizz::PeerCert {
 public:
  explicit OpenSSLPeerCertImpl(folly::ssl::X509UniquePtr cert);

  ~OpenSSLPeerCertImpl() override = default;

  [[nodiscard]] std::string getIdentity() const override;

  void verify(
      SignatureScheme scheme,
      CertificateVerifyContext context,
      folly::ByteRange toBeSigned,
      folly::ByteRange signature) const override;

  [[nodiscard]] folly::ssl::X509UniquePtr getX509() const override;

 protected:
  OpenSSLSignature<T> signature_;
  folly::ssl::X509UniquePtr cert_;
};

} // namespace openssl
} // namespace fizz

#include <fizz/backend/openssl/certificate/OpenSSLPeerCertImpl-inl.h>
