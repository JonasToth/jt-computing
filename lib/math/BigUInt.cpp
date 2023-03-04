#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/container/BitVector.hpp"

#include <algorithm>
#include <cassert>
#include <compare>
#include <istream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <stdexcept>

namespace jt::math {
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

  assert(other.binaryDigits() == binaryDigits());

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

  assert(magnitudeRelation == std::strong_ordering::greater);

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
  assert(value > 0);

  if (binaryDigits() == usize{0}) {
    return *this;
  }
  _bits <<= value;
  return *this;
}

BigUInt &BigUInt::operator>>=(int value) {
  assert(value > 0);

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
  assert(b != 0U);
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

template <u8 Base> char digitToChar(u8 digit) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");
  if (digit >= Base) {
    throw std::out_of_range{"Digit must be < Base"};
  }
  if constexpr (Base == 2 || Base == 8 || Base == 10) {
    return static_cast<char>('0' + digit);
  } else if constexpr (Base == 16) {
    return digit < 10 ? static_cast<char>('0' + digit)
                      : static_cast<char>((digit - 10) + 'a');
  }

  assert(false && "Unreachable");
}

template <u8 Base> std::string writeInBase(BigUInt n) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");

  std::string reverseDigits;
  const auto base = BigUInt{Base};
  while (n > 0U) {
    auto [quotient, remainder] = divmod(n, base);
    n                          = std::move(quotient);
    reverseDigits += digitToChar<Base>(remainder.convertTo<u8>());
  }

  std::reverse(reverseDigits.begin(), reverseDigits.end());
  return reverseDigits;
}

std::ostream &operator<<(std::ostream &os, BigUInt n) {
  const auto flags = os.flags();
  if ((flags & std::ios_base::dec) != 0) {
    os << writeInBase<10>(std::move(n));
  } else if ((flags & std::ios_base::oct) != 0) {
    if ((flags & std::ios_base::showbase) != 0) {
      os << "0";
    }
    os << writeInBase<8>(std::move(n));
  } else if ((flags & std::ios_base::hex) != 0) {
    if ((flags & std::ios_base::showbase) != 0) {
      os << "0x";
    }
    os << writeInBase<16>(std::move(n));
  } else {
    assert(false && "Either number base must be configured");
  }
  return os;
}

template <u8 Base>
BigUInt interpretDigitsInBase(const std::vector<u8> &digitsHighestFirst) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");
  const auto base = BigUInt{Base};
  auto result     = 0_N;
  auto position   = 1_N;
  for (u8 digit : std::ranges::reverse_view(digitsHighestFirst)) {
    result += position * digit;
    position *= base;
  }

  return result;
}

template <u8 Base> std::optional<u8> nextDigit(std::istream &is) {
  int c = is.peek();
  if constexpr (Base == 2 || Base == 8 || Base == 10) {
    if (std::isdigit(c)) {
      return static_cast<u8>(is.get() - '0');
    }
  } else if constexpr (Base == 16) {
    if ((c >= '0') && (c <= '9')) {
      (void)is.get();
      return static_cast<u8>(c - '0');
    }
    if ((c >= 'A') && (c <= 'F')) {
      (void)is.get();
      return static_cast<u8>(c - 'A' + 10);
    }
    if ((c >= 'a') && (c <= 'f')) {
      (void)is.get();
      return static_cast<u8>(c - 'a' + 10);
    }
  }
  return std::nullopt;
}

template <u8 Base> BigUInt extractNumber(std::istream &is) {
  std::vector<u8> digitsHighestFirst;
  // Read from the stream as long as the next character is a digit.
  // Extracts each digit into 'digitsHighestFirst'.
  while (auto digit = nextDigit<Base>(is)) {
    digitsHighestFirst.emplace_back(digit.value());
  }
  return interpretDigitsInBase<Base>(digitsHighestFirst);
}

std::istream &operator>>(std::istream &is, BigUInt &n) {
  const auto flags = is.flags();
  if ((flags & std::ios_base::dec) != 0) {
    n = extractNumber<10>(is);
  } else if ((flags & std::ios_base::oct) != 0) {
    n = extractNumber<8>(is);
  } else if ((flags & std::ios_base::hex) != 0) {
    n = extractNumber<16>(is);
  }
  return is;
}

BigUInt operator"" _N(unsigned long long int literal) {
  return BigUInt{literal};
}
BigUInt operator""_N(char const *literal, std::size_t /*len*/) {
  auto r  = 0_N;
  auto ss = std::stringstream{literal};
  ss >> r;
  return r;
}
} // namespace jt::math
