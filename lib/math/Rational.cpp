#include "jt-computing/math/Rational.hpp"

#include "jt-computing/math/NaturalNumberAlgorithms.hpp"

namespace jt::math {
void Rational::reduce() {
  const auto d = gcd(_num.abs(), _denom);
  if (d == 1U) {
    return;
  }
  _num /= d;
  _denom /= d;
}
bool Rational::operator==(const Rational &other) const noexcept {
  if (getNumerator() != other.getNumerator()) {
    return false;
  }
  if (getDenominator() != other.getDenominator()) {
    return false;
  }
  return true;
}
std::strong_ordering Rational::operator<=>(const Rational &other) const {
  if (getNumerator().isNegative() && !other.getNumerator().isNegative()) {
    return std::strong_ordering::less;
  }
  if (!getNumerator().isNegative() && other.getNumerator().isNegative()) {
    return std::strong_ordering::greater;
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

Rational operator"" _Q(long double rational) { return Rational{i64(rational)}; }

std::ostream &operator<<(std::ostream &os, const Rational &z) {
  os << z.getNumerator() << "/" << z.getDenominator();
  return os;
}
} // namespace jt::math
