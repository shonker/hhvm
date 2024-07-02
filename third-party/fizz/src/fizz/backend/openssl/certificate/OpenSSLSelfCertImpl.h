/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/backend/openssl/crypto/signature/Signature.h>
#include <fizz/compression/CertificateCompressor.h>
#include <fizz/protocol/Certificate.h>
#include <folly/io/async/ssl/OpenSSLTransportCertificate.h>

namespace fizz {

class SelfCert;
enum class CertificateVerifyContext;

namespace openssl {

template <KeyType T>
class OpenSSLSelfCertImpl : public SelfCert {
 public:
  /**
   * Private key is the private key associated with the leaf cert.
   * certs is a list of certs in the chain with the leaf first.
   */
  OpenSSLSelfCertImpl(
      folly::ssl::EvpPkeyUniquePtr pkey,
      std::vector<folly::ssl::X509UniquePtr> certs,
      const std::vector<std::shared_ptr<fizz::CertificateCompressor>>&
          compressors = {});

  ~OpenSSLSelfCertImpl() override = default;

  [[nodiscard]] std::string getIdentity() const override;

  [[nodiscard]] std::vector<std::string> getAltIdentities() const override;

  [[nodiscard]] std::vector<SignatureScheme> getSigSchemes() const override;

  [[nodiscard]] CertificateMsg getCertMessage(
      Buf certificateRequestContext = nullptr) const override;

  [[nodiscard]] CompressedCertificate getCompressedCert(
      CertificateCompressionAlgorithm algo) const override;

  [[nodiscard]] Buf sign(
      SignatureScheme scheme,
      CertificateVerifyContext context,
      folly::ByteRange toBeSigned) const override;

  [[nodiscard]] folly::ssl::X509UniquePtr getX509() const override;

 protected:
  // Allows derived classes to handle init
  explicit OpenSSLSelfCertImpl(std::vector<folly::ssl::X509UniquePtr> certs);
  OpenSSLSignature<T> signature_;
  std::vector<folly::ssl::X509UniquePtr> certs_;
  std::map<CertificateCompressionAlgorithm, CompressedCertificate>
      compressedCerts_;
};
} // namespace openssl
} // namespace fizz

#include <fizz/backend/openssl/certificate/OpenSSLSelfCertImpl-inl.h>
