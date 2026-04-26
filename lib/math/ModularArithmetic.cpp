export module jt.Math:ModularArithmetic;

import :Concepts;

export namespace jt::math {
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
} // namespace jt::math
