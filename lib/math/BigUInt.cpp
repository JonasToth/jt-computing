module;

#include "jt-computing/core/Contracts.hpp"

export module jt.Math:BigUInt;

import :NaturalNumberAlgorithms;

import std;

import jt.Core;
import jt.Container;

namespace jt::math {

/// Arbitrary sized unsigned integer type, stored as @c container::BitVector.
/// The vector is always normalized, meaning it does not have leading zeros.
export class BigUInt {
public:
  BigUInt() = default;

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
export std::pair<BigUInt, BigUInt> divmod(BigUInt dividend,
                                          const BigUInt &divisor);

/// Write 'n' to 'os', optionally adhering to the base modifiers.
export std::ostream &operator<<(std::ostream &os, BigUInt n);

/// Parse 'n' from 'is', optionally adhering to the base modifiers.
export std::istream &operator>>(std::istream &is, BigUInt &n);

export BigUInt operator""_N(unsigned long long literal);
export BigUInt operator""_N(char const *literal, std::size_t len);

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

export inline BigUInt operator+(BigUInt a, const BigUInt &b) { return a += b; }
export inline BigUInt operator+(std::unsigned_integral auto a,
                                const BigUInt &b) {
  return BigUInt{a} += b;
}
export inline BigUInt operator+(BigUInt a, std::unsigned_integral auto b) {
  return a += b;
}

export inline BigUInt operator-(BigUInt a, const BigUInt &b) { return a -= b; }
export inline BigUInt operator-(std::unsigned_integral auto a,
                                const BigUInt &b) {
  return BigUInt{a} -= b;
}
export inline BigUInt operator-(BigUInt a, std::unsigned_integral auto b) {
  return a -= b;
}

export inline BigUInt operator*(BigUInt a, const BigUInt &b) { return a *= b; }
export inline BigUInt operator*(std::unsigned_integral auto a,
                                const BigUInt &b) {
  return BigUInt{a} *= b;
}
export inline BigUInt operator*(BigUInt a, std::unsigned_integral auto b) {
  return a *= b;
}

export inline BigUInt operator/(BigUInt a, const BigUInt &b) { return a /= b; }
export inline BigUInt operator/(std::unsigned_integral auto a,
                                const BigUInt &b) {
  return BigUInt{a} /= b;
}
export inline BigUInt operator/(BigUInt a, std::unsigned_integral auto b) {
  return a /= b;
}

export inline BigUInt operator%(BigUInt a, const BigUInt &b) { return a %= b; }
export inline BigUInt operator%(std::unsigned_integral auto a,
                                const BigUInt &b) {
  return BigUInt{a} %= b;
}
export inline BigUInt operator%(BigUInt a, std::unsigned_integral auto b) {
  return a %= b;
}

export inline bool isEven(const BigUInt &n) noexcept { return n.isEven(); }
export inline bool isOdd(const BigUInt &n) noexcept { return n.isOdd(); }

bool BigUInt::operator==(const BigUInt &other) const noexcept {
  return (*this <=> other) == std::strong_ordering::equal;
}
std::strong_ordering BigUInt::operator<=>(const BigUInt &other) const noexcept {
  auto digitsComparison = binaryDigits() <=> other.binaryDigits();
  // - This number has less digits => this number must be smaller than other.
  // - This number has more digits => this number must be bigger than other.
  if (digitsComparison != std::strong_ordering::equal) {
    return digitsComparison;
  }

  CONTRACT_ASSERT(other.binaryDigits() == binaryDigits());

  if (binaryDigits() == usize{0ULL}) {
    return std::strong_ordering::equal;
  }

  for (usize i = binaryDigits(); i > 0; --i) {
    const usize idx = i - 1;
    if (_bits.get(idx) != other._bits.get(idx)) {
      // *this is smaller than @c other if the first differing digit is 'false'.
      // Otherwise @c other is smaller.
      return !_bits.get(idx) ? std::strong_ordering::less
                             : std::strong_ordering::greater;
    }
  }

  // All digits are identical, so this number is not smaller than @c other.
  return std::strong_ordering::equal;
}

BigUInt &BigUInt::operator+=(const BigUInt &other) {
  // A number with length 0 is neutral and will not change this number.
  if (other.binaryDigits() == usize{0ULL}) {
    return *this;
  }

  // 1. Extend the data storage by enough bits to guarantee that the result is
  //    representable.
  while (other.binaryDigits() > binaryDigits()) {
    _bits.push_back(false);
  }
  // Push one bit as a potential carry bit.
  _bits.push_back(false);

  // 2. Execute the addition of the other number digits to @c this until
  //    @c other is exhausted.
  bool carry            = false;
  usize i               = 0;
  const usize minDigits = other.binaryDigits();
  for (; i < minDigits; ++i) {
    const bool digitSum  = other._bits.get(i) ^ _bits.get(i);
    const bool nextCarry = other._bits.get(i) && _bits.get(i);

    _bits.set(i, digitSum ^ carry);
    carry = nextCarry || (digitSum && carry);
  }

  // 3. There might be an overflow, meaning the @c carry is positive after the
  //    addition of the minimal amount of digits. The other number is exhausted,
  //    but the carry must be added to @c this properly.
  for (const usize maxDigits = binaryDigits(); i < maxDigits; ++i) {
    const bool digitSum  = carry ^ _bits.get(i);
    const bool nextCarry = carry && _bits.get(i);

    _bits.set(i, digitSum);
    carry = nextCarry;

    if (!carry) {
      break;
    }
  }

  _bits.normalize();
  return *this;
}

BigUInt &BigUInt::operator-=(const BigUInt &other) {
  const auto magnitudeRelation = *this <=> other;

  // Subtraction on natural numbers is only defined for @c Bigger - Smaller
  // numbers. Negative numbers can not be represented with @c BigUInt.
  if (magnitudeRelation == std::strong_ordering::less) {
    throw std::domain_error{"unsigned-subtraction would yield negative result"};
  }

  // Both number are equal. The result of subtraction is the neutral element, @c
  // 0U.
  if (magnitudeRelation == std::strong_ordering::equal) {
    *this = BigUInt{0U};
    return *this;
  }

  // A number with length 0 is the neutral element @c 0U and will not change
  // this number.
  if (other.binaryDigits() == usize{0ULL}) {
    return *this;
  }

  CONTRACT_ASSERT(magnitudeRelation == std::strong_ordering::greater);

  // 1. Subtract @c other from @c this by subtracting each digit individually.
  //    If @c 0 - 1 is executed, the subtraction "borrows" from the next digit.
  bool borrow           = false;
  usize i               = 0;
  const usize minDigits = other.binaryDigits();
  for (; i < minDigits; ++i) {
    const bool digitDifference = other._bits.get(i) ^ _bits.get(i);
    const bool nextBorrow      = other._bits.get(i) && !_bits.get(i);

    _bits.set(i, digitDifference ^ borrow);
    borrow = nextBorrow || (!digitDifference && borrow);
  }

  // 2. There might be an overflow, meaning the @c borrow is positive after the
  //    subtraction of the minimal amount of digits. The other number is
  //    exhausted, but the borrow must be subtracted from @c this properly.
  for (const usize maxDigits = binaryDigits(); i < maxDigits; ++i) {
    const bool digitDifference = borrow ^ _bits.get(i);
    const bool nextBorrow      = borrow && !_bits.get(i);

    _bits.set(i, digitDifference);
    borrow = nextBorrow;

    if (!borrow) {
      break;
    }
  }

  _bits.normalize();
  return *this;
}

static BigUInt egyptianMultiplication(BigUInt n, BigUInt a) {
  const auto one                = BigUInt{1U};
  const auto multiplyAccumulate = [&one](BigUInt accumulator, BigUInt _n,
                                         BigUInt _a) {
    while (true) {
      if (_n.isOdd()) {
        accumulator += _a;
        if (_n == one) {
          return accumulator;
        }
      }
      _n >>= 1;
      _a += _a;
    }
  };

  if (n == one) {
    return a;
  }
  return multiplyAccumulate(BigUInt{0U}, std::move(n), std::move(a));
}

BigUInt &BigUInt::operator*=(const BigUInt &other) {
  if (binaryDigits() == 0U) {
    return *this;
  }
  if (other.binaryDigits() == 0U) {
    _bits = container::BitVector{};
    return *this;
  }
  *this = egyptianMultiplication(*this, other);
  return *this;
}

BigUInt &BigUInt::operator/=(const BigUInt &other) {
  const auto quotient = divmod(*this, other).first;
  *this               = quotient;
  return *this;
}

BigUInt &BigUInt::operator%=(const BigUInt &other) {
  const auto modulus = divmod(*this, other).second;
  *this              = modulus;
  return *this;
}

BigUInt &BigUInt::operator<<=(int value) {
  CONTRACT_ASSERT(value > 0);

  if (binaryDigits() == usize{0}) {
    return *this;
  }
  _bits <<= value;
  return *this;
}

BigUInt &BigUInt::operator>>=(int value) {
  CONTRACT_ASSERT(value > 0);

  if (binaryDigits() == usize{0}) {
    return *this;
  }

  if (usize(value) >= binaryDigits()) {
    _bits = container::BitVector();
  } else {
    _bits >>= value;
  }
  return *this;
}

bool BigUInt::isEven() const noexcept {
  return _bits.size() == 0U || !_bits.get(0U);
}

static BigUInt largestDoubling(const BigUInt &a, BigUInt b) {
  CONTRACT_ASSERT(b != 0U);
  while ((a - b) >= b) {
    b <<= 1;
  }
  return b;
}

std::pair<BigUInt, BigUInt> divmod(BigUInt dividend, const BigUInt &divisor) {
  if (divisor == 0U) {
    throw std::invalid_argument{"divison by zero is not possible"};
  }

  if (dividend == 0U) {
    return {BigUInt{0U}, BigUInt{0U}};
  }
  if (dividend < divisor) {
    return {BigUInt{0U}, dividend};
  }
  BigUInt helper = largestDoubling(dividend, divisor);
  BigUInt quotient{1U};

  dividend -= helper;

  while (helper != divisor) {
    // Half 'c' with a shift.
    helper >>= 1U;
    // Double 'quotient' with a shift.
    quotient <<= 1U;

    if (helper <= dividend) {
      dividend -= helper;
      quotient += 1U;
    }
  }

  return {quotient, dividend};
}

export BigUInt operator""_N(unsigned long long int literal) {
  return BigUInt{literal};
}
export BigUInt operator""_N(char const *literal, std::size_t /*len*/) {
  auto r  = 0_N;
  auto ss = std::stringstream{literal};
  ss >> r;
  return r;
}
} // namespace jt::math
