#pragma once

#include "jt-computing/math/Concepts.hpp"
#include "jt-computing/math/GenericPower.hpp"
#include "jt-computing/math/ModularArithmetic.hpp"
#include "jt-computing/math/NaturalN.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

namespace jt::crypto {

enum class Key {
  Private, ///< Private Key in RSA.
  Public,  ///< Public Key in RSA.
};

template <Key K, math::NaturalNumber N = math::NaturalN> class TextbookRSA {
public:
  TextbookRSA(N modul, N e) : _modulus{std::move(modul)}, _e{std::move(e)} {}

  [[nodiscard]] N apply(N plainText) const {
    return math::power_monoid(plainText, _e, math::multiplies_mod{_modulus});
  }

private:
  N _modulus;
  N _e;
};

template <math::NaturalNumber N = math::NaturalN> class RSASha256Signature {
public:
  explicit RSASha256Signature(TextbookRSA<Key::Private, N> key)
      : _key{std::move(key)} {}

  [[nodiscard]] std::string signEMSA_PKCS1v1_5(std::string_view sha256Hash) const {
    // See "PKCS #1 V2.2: RSA - Section 9.2 EMSA-PKCS1-v1_5"
    std::string rawSignaturePadded =
        // Padding bytes.
        "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
        "fffffffff00"
        // ASN.1 Encoding for the Algorithm ID. Defined in Note (1).
        "3031300d060960864801650304020105000420";

    rawSignaturePadded += sha256Hash;
    auto signature = N{0U};
    std::istringstream{rawSignaturePadded} >> std::hex >> signature;

    const auto encrypted = _key.apply(signature);

    std::ostringstream oss;
    oss << std::hex << encrypted;

    return oss.str();
  }

private:
  TextbookRSA<Key::Private, N> _key;
};
} // namespace jt::crypto
