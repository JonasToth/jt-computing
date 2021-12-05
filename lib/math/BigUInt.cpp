#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/container/BitVector.hpp"

#include <algorithm>
#include <cassert>
#include <compare>

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
      return _bits.get(idx) == false ? std::strong_ordering::less
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
  bool carry = false;
  usize i = 0;
  const usize minDigits = other.binaryDigits();
  for (; i < minDigits; ++i) {
    const bool digitSum = other._bits.get(i) ^ _bits.get(i);
    const bool nextCarry = other._bits.get(i) && _bits.get(i);

    _bits.set(i, digitSum ^ carry);
    carry = nextCarry || (digitSum && carry);
  }

  // 3. There might be an overflow, meaning the @c carry is positive after the
  //    addition of the minimal amount of digits. The other number is exhausted,
  //    but the carry must be added to @c this properly.
  for (const usize maxDigits = binaryDigits(); i < maxDigits; ++i) {
    const bool digitSum = carry ^ _bits.get(i);
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

static BigUInt egyptianMultiplication(BigUInt n, BigUInt a) {
  const auto one = BigUInt{1U};
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
} // namespace jt::math
