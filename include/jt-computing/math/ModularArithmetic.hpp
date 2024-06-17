#ifndef MODULAR_ARITHMETIC_HPP
#define MODULAR_ARITHMETIC_HPP

#include "jt-computing/math/Concepts.hpp"

#include <optional>
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

template <NaturalNumber N> N identity_element(multiplies_mod<N> /*op*/) {
  return N{1U};
}

/// Perform divides of two @c NaturalNumbers modulus @c n.
template <NaturalNumber N> struct divides_mod {
  explicit divides_mod(N n) : modulus{std::move(n)} {}

  N operator()(const N &lhs, const N &rhs) { return (lhs / rhs) % modulus; }

private:
  N modulus;
};

/// Invert @p a under modular division by @p n.
/// If the number is not invertible, return @c std::nullopt.
template <class Integer>
std::optional<Integer> modular_inverse(Integer a, Integer n) {
  // https://www.extendedeuclideanalgorithm.com/multiplicative_inverse.php
  auto t    = Integer{0U};
  auto newt = Integer{1U};
  auto r    = n;
  auto newr = a;

  while (newr != Integer{0U}) {
    auto quotient = r / newr;
    auto nextT    = t - quotient * newt;
    t             = std::move(newt);
    newt          = std::move(nextT);

    auto nextR    = r - quotient * newr;
    r             = newr;
    newr          = std::move(nextR);
  }

  if (r > Integer{1U}) {
    return std::nullopt;
  }
  if (t < 0) {
    return t + n;
  }
  return t;
}

} // namespace jt::math

#endif
