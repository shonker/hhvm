// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

/*

This is the backend for the openssl crypto implementations.
Include this file to use openssl features.

*/

#include <fizz/fizz-config.h>

#include <fizz/backend/openssl/Hasher.h>
#include <fizz/backend/openssl/OpenSSLFactory.h>
#include <fizz/backend/openssl/Properties.h>
#include <fizz/backend/openssl/certificate/CertUtils.h>
#include <fizz/backend/openssl/certificate/OpenSSLPeerCertImpl.h>
#include <fizz/backend/openssl/certificate/OpenSSLSelfCertImpl.h>
#include <fizz/backend/openssl/crypto/ECCurve.h>
#include <fizz/backend/openssl/crypto/OpenSSLKeyUtils.h>
#include <fizz/backend/openssl/crypto/Sha256.h>
#include <fizz/backend/openssl/crypto/Sha384.h>
#include <fizz/backend/openssl/crypto/Sha512.h>
#include <fizz/backend/openssl/crypto/aead/AESGCM128.h>
#include <fizz/backend/openssl/crypto/aead/AESGCM256.h>
#include <fizz/backend/openssl/crypto/aead/AESOCB128.h>
#include <fizz/backend/openssl/crypto/aead/ChaCha20Poly1305.h>
#include <fizz/backend/openssl/crypto/aead/OpenSSLEVPCipher.h>
#include <fizz/backend/openssl/crypto/exchange/OpenSSLKeyExchange.h>
#include <fizz/backend/openssl/crypto/signature/Signature.h>
#include <fizz/crypto/Hkdf.h>
#include <folly/io/IOBuf.h>

namespace fizz::openssl {

template <typename T>
std::unique_ptr<fizz::KeyExchange> makeKeyExchange() {
  return std::make_unique<OpenSSLECKeyExchange>(
      Properties<T>::curveNid, T::keyShareLength);
}

// For use in tests. This is not part of the "backend" compile time
// interface. Other callers should be working with the type-erased
// `KeyExchange` object.
template <typename T>
std::unique_ptr<fizz::openssl::OpenSSLECKeyExchange>
makeOpenSSLECKeyExchange() {
  return std::make_unique<OpenSSLECKeyExchange>(
      Properties<T>::curveNid, T::keyShareLength);
}

template <typename Hash>
HkdfImpl createHkdf() {
  return HkdfImpl(Hash::HashLen, &openssl::Hasher<Hash>::hmac);
}

} // namespace fizz::openssl
