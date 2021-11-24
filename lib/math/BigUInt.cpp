#include "jt-computing/math/BigUInt.hpp"

#include <algorithm>

namespace jt::math {
bool BigUInt::operator==(const BigUInt &other) const noexcept {
  if (other._bits.size() != _bits.size()) {
    return false;
  }

  for (usize i = 0ULL; i < _bits.size(); ++i) {
    if (other._bits.get(i) != _bits.get(i)) {
      return false;
    }
  }

  return true;
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

bool BigUInt::isEven() const noexcept {
  return _bits.size() == 0U || !_bits.get(0U);
}
} // namespace jt::math
