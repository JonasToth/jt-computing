#pragma once

#include "jt-computing/math/Concepts.hpp"
#include "jt-computing/math/GenericPower.hpp"
#include "jt-computing/math/ModularArithmetic.hpp"

#include <memory>

namespace jt::crypto {

template <math::NaturalNumber N> class TextbookRSAPublic {
public:
  TextbookRSAPublic(N modul, N e)
      : _modulus{std::move(modul)}, _e{std::move(e)} {}

  N encrypt(N plainText) {
    return math::power_monoid(plainText, _e, math::multiplies_mod{_modulus});
  }

private:
  N _modulus;
  N _e;
};

template <math::NaturalNumber N> class TextbookRSAPrivate {
public:
  TextbookRSAPrivate(N modul, N d)
      : _modulus{std::move(modul)}, _d{std::move(d)} {}

  N decrypt(N cipherText) {
    return math::power_monoid(cipherText, _d, math::multiplies_mod{_modulus});
  }

private:
  N _modulus;
  N _d;
};
} // namespace jt::crypto
