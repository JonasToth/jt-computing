#ifndef MODULAR_ARITHMETIC_HPP
#define MODULAR_ARITHMETIC_HPP

#include "jt-computing/Types.hpp"
#include "jt-computing/math/Concepts.hpp"

#include <utility>

namespace jt::math {
/// Perform addition of two @c NaturalNumbers modulus @c n.
template <NaturalNumber N> struct plus_mod {
  explicit plus_mod(N n) : modulus{std::move(n)} {}

  N operator()(const N &lhs, const N &rhs) { return (lhs + rhs) % modulus; }

private:
  N modulus;
};

/// Perform subtraction of two @c NaturalNumbers modulus @c n.
template <NaturalNumber N> struct minus_mod {
  explicit minus_mod(N n) : modulus{std::move(n)} {}

  N operator()(const N &lhs, const N &rhs) { return (lhs - rhs) % modulus; }

private:
  N modulus;
};

/// Perform multiplication of two @c NaturalNumbers modulus @c n.
template <NaturalNumber N> struct multiplies_mod {
  explicit multiplies_mod(N n) : modulus{std::move(n)} {}

  N operator()(const N &lhs, const N &rhs) { return (lhs * rhs) % modulus; }

private:
  N modulus;
};

/// Perform divides of two @c NaturalNumbers modulus @c n.
template <NaturalNumber N> struct divides_mod {
  explicit divides_mod(N n) : modulus{std::move(n)} {}

  N operator()(const N &lhs, const N &rhs) { return (lhs / rhs) % modulus; }

private:
  N modulus;
};
} // namespace jt::math

#endif
