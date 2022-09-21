#include "jt-computing/math/BigInt.hpp"

#include <algorithm>
#include <cassert>
#include <compare>

namespace jt::math {
bool BigInt::operator==(const BigInt &other) const noexcept {
  // The signs of both numbers differ, they can not be equal.
  if (isNegative() != other.isNegative()) {
    return false;
  }
  return abs() == other.abs();
}

bool BigInt::operator==(const BigUInt &other) const noexcept {
  // The signs of both numbers differ, they can not be equal.
  if (isNegative()) {
    return false;
  }
  return abs() == other;
}

std::strong_ordering BigInt::operator<=>(const BigInt &other) const noexcept {
  // '*this < 0' && 'other >= 0' => '*this < other'.
  if (isNegative() && !other.isNegative()) {
    return std::strong_ordering::less;
  }

  // 'other < 0' && '*this >= 0' => 'other < *this'.
  if (other.isNegative() && !isNegative()) {
    return std::strong_ordering::greater;
  }

  // The signs must be identical after the first two conditions were false.
  assert(isNegative() == other.isNegative());

  // E.g.: -10 < -9 <==> |-9| <=> |-10| <==> 9 <=> 10
  if (isNegative()) {
    return other.abs() <=> abs();
  }
  return abs() <=> other.abs();
}

std::strong_ordering BigInt::operator<=>(const BigUInt &other) const noexcept {
  // '*this < 0' && 'other >= 0' => '*this < other'.
  if (isNegative()) {
    return std::strong_ordering::less;
  }

  // The signs must be identical after the first two conditions were false.
  assert(!isNegative());

  return abs() <=> other;
}

BigInt &BigInt::operator+=(const BigInt &other) {
  // a) -20 + -20 = -40
  // b) +20 + +20 = +40
  if (isNegative() == other.isNegative()) {
    _val += other.abs();
    return *this;
  }
  assert(isNegative() ^ other.isNegative() &&
         "Signs must differ at this point");

  // The signs of *this will change by this operation.
  if (other.abs() > abs()) {
    _val        = other.abs() - abs();
    _isNegative = !isNegative();
  } else {
    _val -= other.abs();
  }

  // Ensure that no negative zero can be created by calculating -20 + 20.
  if (_val.binaryDigits() == 0U) {
    _isNegative = false;
  }

  return *this;
}

BigInt &BigInt::operator*=(const BigInt &other) {
  _val *= other.abs();
  _isNegative = isNegative() ^ other.isNegative();

  // Ensure that no negative zero can be created by calculating -20 + 20.
  if (_val.binaryDigits() == 0U) {
    _isNegative = false;
  }
  return *this;
}

BigInt &BigInt::operator/=(const BigInt &other) {
  _val /= other.abs();
  _isNegative = isNegative() ^ other.isNegative();

  // Ensure that no negative zero can be created by calculating -20 + 20.
  if (_val.binaryDigits() == 0U) {
    _isNegative = false;
  }
  return *this;
}
} // namespace jt::math
