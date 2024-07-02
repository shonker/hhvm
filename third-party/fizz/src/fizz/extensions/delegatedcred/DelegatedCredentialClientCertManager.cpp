/*
 *  Copyright (c) 2019-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */
#include <fizz/extensions/delegatedcred/DelegatedCredentialClientCertManager.h>
#include <fizz/extensions/delegatedcred/Types.h>

namespace fizz {
namespace extensions {

fizz::client::CertManager::CertMatch
DelegatedCredentialClientCertManager::getCert(
    const folly::Optional<std::string>& sni,
    const std::vector<SignatureScheme>& supportedSigSchemes,
    const std::vector<SignatureScheme>& peerSigSchemes,
    const std::vector<Extension>& peerExtensions) const {
  auto credential = getExtension<DelegatedCredentialSupport>(peerExtensions);

  if (credential) {
    auto dcRes = dcMgr_.getCert(
        sni,
        supportedSigSchemes,
        credential->supported_signature_algorithms,
        peerExtensions);
    if (dcRes) {
      return dcRes;
    }
  }
  return CertManager::getCert(
      sni, supportedSigSchemes, peerSigSchemes, peerExtensions);
}

void DelegatedCredentialClientCertManager::addDelegatedCredential(
    std::shared_ptr<SelfCert> cert) {
  VLOG(8) << "Adding delegated credential";
  dcMgr_.addCert(std::move(cert));
}

void DelegatedCredentialClientCertManager::addDelegatedCredentialAndOverride(
    std::shared_ptr<SelfCert> cert) {
  VLOG(8) << "Adding delegated credential";
  dcMgr_.addCertAndOverride(std::move(cert));
}

} // namespace extensions
} // namespace fizz
