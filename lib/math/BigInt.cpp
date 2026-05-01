module;

#include "jt-computing/core/Contracts.hpp"

export module jt.Math:BigInt;

import :BigUInt;

import std;
import jt.Core;

export namespace jt::math {

/// Stores a @c BigUInt together with a sign bit.
/// Refines the mathematical operations accordingly.
/// @note the 0 is always positive, "-0" has the same representation as "0".
class BigInt {
public:
  BigInt() = default;

  /// Construct a @c BigInt from any builtin integer type.
  template <std::signed_integral Z> explicit BigInt(Z value);
  template <std::unsigned_integral Z> explicit BigInt(Z value) : _val{value} {}
  explicit BigInt(BigUInt value) : _val{std::move(value)} {}

  /// Compare the sign to @c other and continue with comparing the value itself.
  bool operator==(const BigInt &other) const noexcept;
  bool operator==(const BigUInt &other) const noexcept;
  bool operator==(std::unsigned_integral auto other) const noexcept;
  bool operator==(std::signed_integral auto other) const noexcept;

  /// First compares the signs of both numbers and then their absolute value
  /// accordingly.
  std::strong_ordering operator<=>(const BigInt &other) const noexcept;
  std::strong_ordering operator<=>(const BigUInt &other) const noexcept;
  std::strong_ordering
  operator<=>(std::unsigned_integral auto other) const noexcept;
  std::strong_ordering
  operator<=>(std::signed_integral auto other) const noexcept;

  /// Implement assign-add with another @c BigInt.
  BigInt &operator+=(const BigInt &other);
  /// Implement assign-add with another @c BigUInt.
  BigInt &operator+=(BigUInt other);
  /// Implement assign-add with another integral value by constructing a
  /// @c BigInt and using the generalized implementation.
  BigInt &operator+=(std::integral auto value);

  /// Invert the sign of the @c BigInt.
  BigInt &operator-() noexcept;

  /// Implement assign-subtract with another @c BigInt.
  BigInt &operator-=(BigInt other);
  /// Implement assign-subtract with another @c BigUInt.
  BigInt &operator-=(BigUInt other);
  /// Implement assign-subtract with another integral value by constructing a
  /// @c BigInt and using the generalized implementation.
  BigInt &operator-=(std::integral auto value);

  /// Implement assign-multiply with another @c BigInt.
  BigInt &operator*=(const BigInt &other);
  /// Implement assign-multiply with another @c BigUInt.
  BigInt &operator*=(const BigUInt &other);
  /// Implement assign-multiply with another integral value by constructing a
  /// @c BigInt and using the generalized implementation.
  BigInt &operator*=(std::integral auto value);

  /// Implement assign-multiply with another @c BigInt.
  BigInt &operator/=(const BigInt &other);
  /// Implement assign-multiply with another @c BigUInt.
  BigInt &operator/=(const BigUInt &other);
  /// Implement assign-multiply with another integral value by constructing a
  /// @c BigInt and using the generalized implementation.
  BigInt &operator/=(std::integral auto value);

  /// @returns @c true if the number is smaller 0. It is not possible to have a
  ///          negative 0 by definition of this type.
  [[nodiscard]] bool isNegative() const noexcept { return _isNegative; }

  /// @returns @c true if the number is divisible by 2.
  [[nodiscard]] bool isEven() const noexcept { return _val.isEven(); }

  /// @returns a reference to the internal @c BigUInt, which is also the
  ///          absolute value of the integer.
  [[nodiscard]] const BigUInt &abs() const noexcept { return _val; }

  /// Switch the sign of the number.
  void negate() noexcept { _isNegative = !isNegative(); }

private:
  bool _isNegative{false};
  BigUInt _val{0U};
};

inline BigInt operator""_Z(unsigned long long literal) {
  return BigInt{literal};
}

/// Allow to change the sign of a 'BigUInt' to create a whole number from a
/// natural number.
inline BigInt operator-(BigUInt n) noexcept {
  auto r = BigInt{std::move(n)};
  return -r;
}

/// Write 'z' to 'os', optionally adhering to the base modifiers.
/// @sa operator<<(std::ostream&, BigUInt)
std::ostream &operator<<(std::ostream &os, const BigInt &z);

/// Parse 'z' from 'is', optionally adhering to the base modifiers.
/// @sa operator>>(std::istream&, BigUInt&)
std::istream &operator>>(std::istream &is, BigInt &z);

template <std::signed_integral Z> auto castToUnsigned(Z value) {
  return static_cast<std::make_unsigned_t<Z>>(std::abs(value));
}
template <std::signed_integral Z>
BigInt::BigInt(Z value) : _isNegative{value < 0}, _val{castToUnsigned(value)} {}

bool BigInt::operator==(std::signed_integral auto other) const noexcept {
  return *this == BigInt{other};
}
bool BigInt::operator==(std::unsigned_integral auto other) const noexcept {
  return *this == BigInt{other};
}

std::strong_ordering
BigInt::operator<=>(std::unsigned_integral auto other) const noexcept {
  return *this <=> BigInt{other};
}
std::strong_ordering
BigInt::operator<=>(std::signed_integral auto other) const noexcept {
  return *this <=> BigInt{other};
}

inline BigInt &BigInt::operator+=(BigUInt other) {
  return *this += BigInt{std::move(other)};
}
BigInt &BigInt::operator+=(std::integral auto value) {
  return *this += BigInt{value};
}

inline BigInt &BigInt::operator-() noexcept {
  negate();
  return *this;
}
inline BigInt &BigInt::operator-=(BigInt other) { return *this += -other; }
inline BigInt &BigInt::operator-=(BigUInt other) {
  return *this += -BigInt{std::move(other)};
}
BigInt &BigInt::operator-=(std::integral auto value) {
  return *this += -BigInt{value};
}

inline BigInt &BigInt::operator*=(const BigUInt &other) {
  _val *= other;
  return *this;
}
BigInt &BigInt::operator*=(std::integral auto value) {
  return *this *= BigInt{value};
}

inline BigInt &BigInt::operator/=(const BigUInt &other) {
  _val /= other;
  return *this;
}
BigInt &BigInt::operator/=(std::integral auto value) {
  return *this /= BigInt{value};
}

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
  CONTRACT_ASSERT(isNegative() == other.isNegative());

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
  CONTRACT_ASSERT(!isNegative());

  return abs() <=> other;
}

BigInt &BigInt::operator+=(const BigInt &other) {
  // a) -20 + -20 = -40
  // b) +20 + +20 = +40
  if (isNegative() == other.isNegative()) {
    _val += other.abs();
    return *this;
  }
  CONTRACT_ASSERT(isNegative() ^ other.isNegative() &&
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

std::ostream &operator<<(std::ostream &os, const BigInt &z) {
  if (z.isNegative()) {
    os << "-";
  }
  os << z.abs();
  return os;
}
std::istream &operator>>(std::istream &is, BigInt &z) {
  bool negative = false;

  // Consume the sign.
  switch (is.peek()) {
  case '-':
    negative = true;
    (void)is.get();
    break;
  case '+':
    negative = false;
    (void)is.get();
    break;
  }
  BigUInt n{0U};
  is >> n;
  if (negative) {
    z = -BigInt{std::move(n)};
  } else {
    z = BigInt{std::move(n)};
  }
  return is;
}
} // namespace jt::math
