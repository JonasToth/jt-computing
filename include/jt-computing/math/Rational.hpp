#ifndef RATIONAL_HPP_12903ASD
#define RATIONAL_HPP_12903ASD

#include "jt-computing/math/BigInt.hpp"
#include "jt-computing/math/BigUInt.hpp"

namespace jt::math {

class Rational {
public:
  Rational() = default;

  template <std::regular N1> Rational(N1 numerator, const BigInt &denominator);
  template <std::regular N1>
  explicit Rational(N1 numerator, BigUInt denominator = 1_N);

  /// Compare the sign to @c other and continue with comparing the value itself.
  bool operator==(const Rational &other) const noexcept;
  /// Compare the sign to @c other and continue with comparing the value itself.
  template <std::regular N> bool operator==(const N &other) const;

  std::strong_ordering operator<=>(const Rational &other) const;

  Rational &operator+=(const Rational &other);
  Rational &operator-=(const Rational &other);
  Rational &operator*=(const Rational &other);
  Rational &operator/=(const Rational &other);
  Rational &operator-() noexcept;

  /// Returns a reference to the _signed_ whole number representing the
  /// numerator. It is always reduced.
  [[nodiscard]] const BigInt &getNumerator() const noexcept { return _num; }

  /// Returns a reference to the _unsigned_ natural number, the denominator.
  /// It is always reduced.
  [[nodiscard]] const BigUInt &getDenominator() const noexcept {
    return _denom;
  }

private:
  /// Reduce numerator and denominator to become inreducible.
  void reduce();

  BigInt _num{0U};
  BigUInt _denom{1U};
};

inline Rational operator"" _Q(unsigned long long number) {
  return Rational{number, 1_Z};
}
// https://de.wikipedia.org/wiki/Stellenwertsystem#Darstellung_rationaler_Zahlen
// inline Rational operator"" _Q(long double rational) { return Rational{}; }

/// Write 'z' to 'os', optionally adhering to the base modifiers.
/// @sa operator<<(std::ostream&, BigUInt)
std::ostream &operator<<(std::ostream &os, const Rational &z);

/// Parse 'z' from 'is', optionally adhering to the base modifiers.
/// @sa operator>>(std::istream&, BigUInt&)
// std::istream &operator>>(std::istream &is, Rational &z);

template <std::regular N1>
Rational::Rational(N1 numerator, const BigInt &denominator)
    : _num{std::move(numerator)}, _denom{denominator.abs()} {
  if (_denom == 0U) {
    throw std::invalid_argument{"Division by 0 detected"};
  }
  if (denominator.isNegative()) {
    _num.negate();
  }
  reduce();
}

template <std::regular N1>
Rational::Rational(N1 numerator, BigUInt denominator)
    : _num{std::move(numerator)}, _denom{std::move(denominator)} {
  if (_denom == 0U) {
    throw std::invalid_argument{"Division by 0 detected"};
  }
  reduce();
}
template <std::regular N> bool Rational::operator==(const N &other) const {
  const auto tmp = Rational{other};
  return *this == tmp;
}

} // namespace jt::math

#endif
