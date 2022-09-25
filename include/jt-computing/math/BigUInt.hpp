#ifndef BIGUINT_HPP_83XPJVDC
#define BIGUINT_HPP_83XPJVDC

#include "jt-computing/Types.hpp"
#include "jt-computing/container/BitVector.hpp"

#include <compare>
#include <concepts>
#include <iosfwd>
#include <limits>
#include <utility>

namespace jt::math {

/// Arbitrary sized unsigned integer type, stored as @c container::BitVector.
/// The vector is always normalized, meaning it does not have leading zeros.
class BigUInt {
public:
  /// Construct the number from a builtin unsigned integer @c value.
  explicit BigUInt(std::unsigned_integral auto value);

  /// Returns the number of bits this number requires.
  [[nodiscard]] usize binaryDigits() const noexcept { return _bits.size(); }

  /// Compare all digits of both @c BigUInt and return @c true if they are
  /// identical.
  bool operator==(const BigUInt &other) const noexcept;
  bool operator==(std::unsigned_integral auto other) const noexcept;

  /// Compare the length of both numbers, because both are canonicalized to not
  /// have leading zeros.
  std::strong_ordering operator<=>(const BigUInt &other) const noexcept;
  std::strong_ordering
  operator<=>(std::unsigned_integral auto other) const noexcept;

  /// Implement assign-add with another @c BigUInt.
  BigUInt &operator+=(const BigUInt &other);
  /// Implement assign-add with another @c unsigned type by constructing a @c
  /// BigUInt and using the generalized implementation.
  BigUInt &operator+=(std::unsigned_integral auto value);

  /// Implement assign-subtract with another @c BigUInt. The result must be a
  /// non-negative number.
  /// @note The algorithm uses borrowing and not a complement represenation.
  /// @throws @c std::domain_error if @c other is bigger than @c this.
  BigUInt &operator-=(const BigUInt &other);
  /// Implement an overload for assign-subtract for builtin types.
  /// @sa operator-=(const BigUInt& other)
  /// @throws @c std::domain_error if @c value is bigger than @c this.
  BigUInt &operator-=(std::unsigned_integral auto value);

  /// Implement assign-multiply with another @c BigUInt.
  BigUInt &operator*=(const BigUInt &other);
  /// Implement assign-multiply with another @c unsigned type by constructing a
  /// @c BigUInt and using the generalized implementation.
  BigUInt &operator*=(std::unsigned_integral auto value);

  /// Implement assign-division with another @c BigUInt that drops the remainder
  /// of the division.
  /// @sa divmod
  /// @throws std::invalid_argument if @c other == 0U
  BigUInt &operator/=(const BigUInt &other);
  /// Implement assign-division with another @c unsigned type by constructing a
  /// @c BigUInt and using the generalized implementation.
  BigUInt &operator/=(std::unsigned_integral auto value);

  /// Implement assign-modulo-division with another @c BigUInt that drops the
  /// quotient of the division and assigns the remainder to *this.
  /// @sa divmod
  /// @throws std::invalid_argument if @c other == 0U
  BigUInt &operator%=(const BigUInt &other);
  /// Implement assign-modulo-division with another @c unsigned type by
  /// constructing a @c BigUInt and using the generalized implementation.
  BigUInt &operator%=(std::unsigned_integral auto value);

  /// Shifts the underlying @c BitVector by @c value positions to the left.
  /// This doubles the @c BigUInt @c value times.
  /// @pre value > 0
  BigUInt &operator<<=(int value);

  /// Shifts the underlying @c BitVector by @c value positions to the right.
  /// This halfes the @c BigUInt @c value times and drops the remainder.
  /// @pre value > 0
  BigUInt &operator>>=(int value);

  /// Returns @c true if the integer is either 0 or it least significant bit is
  /// @c true.
  [[nodiscard]] bool isEven() const noexcept;

  /// Returns @c !isEven().
  [[nodiscard]] bool isOdd() const noexcept { return !isEven(); }

  /// Tries to convert to a builtin type. If the value does not fit, it throws
  /// an 'std::out_of_range' exception instead of performing a narrowing
  /// conversion.
  template <std::unsigned_integral Target> Target convertTo() const;

private:
  container::BitVector _bits;
};

/// Perfom natural number division and return the quotient and the
/// modulus/remainder.
/// @throws std::invalid_argument if @c divisor == 0.
/// @returns {quotient, modulus}
std::pair<BigUInt, BigUInt> divmod(BigUInt dividend, const BigUInt &divisor);

template <std::unsigned_integral NaturalNumber>
std::pair<NaturalNumber, NaturalNumber> divmod(NaturalNumber dividend,
                                               NaturalNumber divisor) {
  return {dividend / divisor, dividend % divisor};
}

std::ostream &operator<<(std::ostream &os, BigUInt n);

std::istream &operator>>(std::istream &is, BigUInt &n);

BigUInt operator"" _N(unsigned long long literal);

BigUInt::BigUInt(std::unsigned_integral auto value) : _bits{value} {
  _bits.normalize();
}

bool BigUInt::operator==(std::unsigned_integral auto other) const noexcept {
  return *this == BigUInt{other};
}

std::strong_ordering
BigUInt::operator<=>(std::unsigned_integral auto other) const noexcept {
  return *this <=> BigUInt{other};
}

BigUInt &BigUInt::operator+=(std::unsigned_integral auto value) {
  return *this += BigUInt{value};
}

BigUInt &BigUInt::operator-=(std::unsigned_integral auto value) {
  return *this -= BigUInt{value};
}

BigUInt &BigUInt::operator*=(std::unsigned_integral auto value) {
  return *this *= BigUInt{value};
}

BigUInt &BigUInt::operator/=(std::unsigned_integral auto value) {
  return *this /= BigUInt{value};
}

template <std::unsigned_integral Target> Target BigUInt::convertTo() const {
  Target result{0U};
  if (*this > std::numeric_limits<Target>::max()) {
    throw std::out_of_range{"Conversion would narrow"};
  }

  for (usize i = 0U; i < _bits.size(); i++) {
    if (_bits.get(i)) {
      result |= (1U << i);
    }
  }

  return result;
}

inline BigUInt operator+(BigUInt a, const BigUInt &b) { return a += b; }
inline BigUInt operator+(std::unsigned_integral auto a, const BigUInt &b) {
  return BigUInt{a} += b;
}
inline BigUInt operator+(BigUInt a, std::unsigned_integral auto b) {
  return a += b;
}

inline BigUInt operator-(BigUInt a, const BigUInt &b) { return a -= b; }
inline BigUInt operator-(std::unsigned_integral auto a, const BigUInt &b) {
  return BigUInt{a} -= b;
}
inline BigUInt operator-(BigUInt a, std::unsigned_integral auto b) {
  return a -= b;
}

inline BigUInt operator*(BigUInt a, const BigUInt &b) { return a *= b; }
inline BigUInt operator*(std::unsigned_integral auto a, const BigUInt &b) {
  return BigUInt{a} *= b;
}
inline BigUInt operator*(BigUInt a, std::unsigned_integral auto b) {
  return a *= b;
}

inline BigUInt operator/(BigUInt a, const BigUInt &b) { return a /= b; }
inline BigUInt operator/(std::unsigned_integral auto a, const BigUInt &b) {
  return BigUInt{a} /= b;
}
inline BigUInt operator/(BigUInt a, std::unsigned_integral auto b) {
  return a /= b;
}

inline BigUInt operator%(BigUInt a, const BigUInt &b) { return a %= b; }
inline BigUInt operator%(std::unsigned_integral auto a, const BigUInt &b) {
  return BigUInt{a} %= b;
}
inline BigUInt operator%(BigUInt a, std::unsigned_integral auto b) {
  return a %= b;
}

} // namespace jt::math

#endif /* end of include guard: BIGUINT_HPP_83XPJVDC */
