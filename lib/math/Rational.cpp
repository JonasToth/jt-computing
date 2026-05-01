export module jt.Math:Rational;

import :BigInt;
import :BigUInt;

import std;
import jt.Core;

using namespace std;

export namespace jt::math {

class Rational {
public:
  Rational() = default;

  template <regular N1> Rational(N1 numerator, const BigInt &denominator);

  /// Compare the sign to @c other and continue with comparing the value itself.
  bool operator==(const Rational &other) const noexcept = default;

  strong_ordering operator<=>(const Rational &other) const;

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

inline Rational operator""_Q(unsigned long long number) {
  return Rational{number, 1_Z};
}
// https://de.wikipedia.org/wiki/Stellenwertsystem#Darstellung_rationaler_Zahlen
Rational operator""_Q(long double rational);

/// Write 'z' to 'os', optionally adhering to the base modifiers.
/// @sa operator<<(ostream&, BigUInt)
ostream &operator<<(ostream &os, const Rational &z);

/// Parse 'z' from 'is', optionally adhering to the base modifiers.
/// @sa operator>>(istream&, BigUInt&)
// istream &operator>>(istream &is, Rational &z);

template <regular N1>
Rational::Rational(N1 numerator, const BigInt &denominator)
    : _num{move(numerator)}, _denom{denominator.abs()} {
  if (_denom == 0U) {
    throw invalid_argument{"Division by 0 detected"};
  }
  if (denominator.isNegative()) {
    _num.negate();
  }
  reduce();
}

void Rational::reduce() {
  const auto d = gcd(_num.abs(), _denom);
  if (d == 1U) {
    return;
  }
  _num /= d;
  _denom /= d;
}

strong_ordering Rational::operator<=>(const Rational &other) const {
  if (getNumerator().isNegative() && !other.getNumerator().isNegative()) {
    return strong_ordering::less;
  }
  if (!getNumerator().isNegative() && other.getNumerator().isNegative()) {
    return strong_ordering::greater;
  }
  const auto denom1         = BigInt{getDenominator()};
  const auto denom2         = BigInt{other.getDenominator()};

  auto extendOtherNumerator = BigInt{other.getNumerator()};
  extendOtherNumerator *= denom1;

  auto extendNumerator = BigInt{getNumerator()};
  extendNumerator *= denom2;

  return extendNumerator <=> extendOtherNumerator;
}

Rational &Rational::operator+=(const Rational &other) {
  auto copy{other.getNumerator()};
  copy *= getDenominator();

  _num *= other.getDenominator();
  _num += copy;

  _denom *= other.getDenominator();

  reduce();
  return *this;
}

Rational &Rational::operator-=(const Rational &other) {
  auto copy{other.getNumerator()};
  copy *= getDenominator();

  _num *= other.getDenominator();
  _num -= copy;

  _denom *= other.getDenominator();

  reduce();
  return *this;
}

Rational &Rational::operator*=(const Rational &other) {
  _num *= other.getNumerator();
  _denom *= other.getDenominator();

  reduce();
  return *this;
}

Rational &Rational::operator/=(const Rational &other) {
  _num *= other.getDenominator();
  _denom *= other.getNumerator().abs();
  if (other.getNumerator().isNegative()) {
    _num.negate();
  }

  reduce();
  return *this;
}

Rational &Rational::operator-() noexcept {
  _num.negate();
  return *this;
}

Rational operator""_Q(long double rational) {
  return Rational{i64(rational), 1_Z};
}

ostream &operator<<(ostream &os, const Rational &z) {
  os << z.getNumerator() << "/" << z.getDenominator();
  return os;
}
} // namespace jt::math
