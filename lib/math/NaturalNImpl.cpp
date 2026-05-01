module;

#include "jt-computing/core/Contracts.hpp"

module jt.Math:NaturalN.Impl;

import :GenericPower;
import :NaturalN;
import :NumberIO;

import std;
import jt.Core;

using namespace std;

namespace jt::math {

template <unsigned_integral Target> Target NaturalN::convertTo() const {
  Target result{0U};
  if (_digits.empty()) {
    return result;
  }
  CONTRACT_ASSERT(!_digits.empty());
  if constexpr (is_same_v<Target, u64>) {
    if (_digits.size() > 2) {
      throw out_of_range{"Conversion would narrow"};
    }
    result += !_digits.empty() ? Target{_digits[0]} : 0U;
    result += _digits.size() > 1U ? Target{_digits[1]} : 0U;
  }
  if constexpr (is_same_v<Target, u32>) {
    if (_digits.size() > 1) {
      throw out_of_range{"Conversion would narrow"};
    }
    return _digits[0];
  }
  if constexpr (is_same_v<Target, u16>) {
    if (_digits.size() > 1 || _digits[0] > numeric_limits<u16>::max()) {
      throw out_of_range{"Conversion would narrow"};
    }
    return Target(_digits[0]);
  }
  if constexpr (is_same_v<Target, u8>) {
    if (_digits.size() > 1 || _digits[0] > numeric_limits<u8>::max()) {
      throw out_of_range{"Conversion would narrow"};
    }
    return Target(_digits[0]);
  }
  CONTRACT_ASSERT(false &&
                  "Unreachable, because all integral types are handled");
}

bool NaturalN::operator==(const NaturalN &other) const noexcept {
  return (*this <=> other) == strong_ordering::equal;
}

strong_ordering NaturalN::operator<=>(const NaturalN &other) const noexcept {
  auto digitsComparison = _digits.size() <=> other._digits.size();
  // - This number has less digits => this number must be smaller than other.
  // - This number has more digits => this number must be bigger than other.
  if (digitsComparison != strong_ordering::equal) {
    return digitsComparison;
  }

  CONTRACT_ASSERT(_digits.size() == other._digits.size());

  if (_digits.empty()) {
    return strong_ordering::equal;
  }

  for (usize i = _digits.size(); i > 0; --i) {
    const auto idx = i - 1;
    const auto &d1 = _digits[idx];
    const auto &d2 = other._digits[idx];
    if (d1 != d2) {
      return d1 < d2 ? strong_ordering::less : strong_ordering::greater;
    }
  }

  // All digits are identical, so this number is not smaller than @c other.
  return strong_ordering::equal;
}

NaturalN &NaturalN::operator+=(const NaturalN &other) {
  // A number with length 0 is neutral and will not change this number.
  if (other._digits.empty()) {
    return *this;
  }

  // 1. Extend the data storage by enough digits to guarantee that the result is
  //    representable.
  if (other._digits.size() > _digits.size()) {
    _digits.resize(other._digits.size());
  }
  _digits.emplace_back(u32{0U});

  // 2. Execute the addition of the other number digits to @c this until
  //    @c other is exhausted.
  u32 carry = 0U;
  usize i   = 0U;
  for (; i < other._digits.size(); ++i) {
    const u64 sum = u64{_digits[i]} + u64{other._digits[i]} + u64{carry};
    carry         = static_cast<u32>(sum >> u32(bitsPerDigit));
    _digits[i]    = static_cast<u32>(sum);
  }

  for (const usize maxDigits = _digits.size(); i < maxDigits; ++i) {
    const u64 sum = u64{_digits[i]} + u64{carry};
    carry         = static_cast<u32>(sum >> u32(bitsPerDigit));
    _digits[i]    = static_cast<u32>(sum);

    if (carry == 0U) {
      break;
    }
  }
  _normalize();
  return *this;
}

NaturalN &NaturalN::operator-=(const NaturalN &other) {
  const auto magnitudeRelation = *this <=> other;

  // Subtraction on natural numbers is only defined for @c Bigger - Smaller
  // numbers. Negative numbers can not be represented with @c BigUInt.
  if (magnitudeRelation == strong_ordering::less) {
    throw domain_error{"unsigned-subtraction would yield negative result"};
  }

  // Both number are equal. The result of subtraction is the neutral element, @c
  // 0U.
  if (magnitudeRelation == strong_ordering::equal) {
    *this = NaturalN{0U};
    return *this;
  }

  // A number with length 0 is the neutral element @c 0U and will not change
  // this number.
  if (other._digits.size() == usize{0U}) {
    return *this;
  }

  CONTRACT_ASSERT(magnitudeRelation == strong_ordering::greater);

  // 1. Subtract @c other from @c this by subtracting each digit individually.
  //    If @c 0 - 1 is executed, the subtraction "borrows" from the next digit.
  u32 borrow = 0U;
  usize i    = 0;
  for (; i < other._digits.size(); ++i) {
    const u32 diff             = _digits[i] - other._digits[i];
    const u32 nextBorrow       = other._digits[i] > _digits[i] ? 1U : 0U;

    const u32 borrowed         = diff - borrow;
    const u32 additionalBorrow = borrow > diff ? 1U : 0U;

    _digits[i]                 = borrowed;
    borrow                     = nextBorrow + additionalBorrow;
  }

  // 2. There might be an overflow, meaning the @c borrow is positive after the
  //    subtraction of the minimal amount of digits. The other number is
  //    exhausted, but the borrow must be subtracted from @c this properly.
  for (const usize maxDigits = _digits.size(); i < maxDigits; ++i) {
    const u32 diff       = _digits[i] - borrow;
    const u32 nextBorrow = borrow > _digits[i] ? 1U : 0U;
    _digits[i]           = diff;
    borrow               = nextBorrow;

    if (borrow == u32{0U}) {
      break;
    }
  }

  _normalize();
  return *this;
}

NaturalN &NaturalN::operator*=(const NaturalN &other) {
  if (_digits.empty()) {
    return *this;
  }
  if (other._digits.empty()) {
    _digits.clear();
    return *this;
  }
#if 0
    return *this = power_monoid(*this, other, plus<NaturalN>{});
#else
  CONTRACT_ASSERT(!_digits.empty());
  CONTRACT_ASSERT(!other._digits.empty());

  auto result = NaturalN{0U};
  for (usize j = 0; j < other._digits.size(); ++j) {
    auto intermediary = NaturalN{0U};

    for (usize i = 0; i < _digits.size(); ++i) {
      auto product = NaturalN{u64{_digits[i]} * u64{other._digits[j]}};
      product <<= int(j + i) * bitsPerDigit;
      intermediary += product;
    }
    result += intermediary;
  }
  return *this = result;
#endif
}
NaturalN &NaturalN::operator/=(const NaturalN &other) {
  if (other == 2_U) {
    return *this >>= 1;
  }
  return *this = divmod(*this, other).first;
}
NaturalN &NaturalN::operator%=(const NaturalN &other) {
  return *this = divmod(*this, other).second;
}
NaturalN &NaturalN::operator<<=(int value) {
  CONTRACT_ASSERT(value >= 0);

  const auto newDigits   = value / bitsPerDigit;
  const auto bitsToShift = value % bitsPerDigit;

  if (newDigits > 0) {
    _digits.insert(_digits.begin(), static_cast<usize>(newDigits), 0U);
  }
  if (bitsToShift == 0 || _digits.empty()) {
    return *this;
  }

  const auto shiftPos  = u32(bitsPerDigit - bitsToShift);
  const auto upperMask = numeric_limits<u32>::max() << shiftPos;

  if (countl_zero(_digits.back()) < bitsToShift) {
    _digits.emplace_back(0U);
  }

  auto carryOver = u32{0U};
  for (auto &digit : _digits) {
    const auto nextCarryOver = (digit & upperMask) >> shiftPos;
    digit <<= u32(bitsToShift);
    digit |= carryOver;
    carryOver = nextCarryOver;
  }

  CONTRACT_ASSERT(_digits.back() != 0U &&
                  "Inserting a zero digit is only done if necessary");
  return *this;
}
NaturalN &NaturalN::operator>>=(int value) {
  CONTRACT_ASSERT(value >= 0);

  const auto removeDigits = value / bitsPerDigit;
  const auto bitsToShift  = value % bitsPerDigit;

  if (usize(removeDigits) >= _digits.size()) {
    _digits.clear();
    return *this;
  }

  CONTRACT_ASSERT(usize(removeDigits) < _digits.size());
  _digits.erase(_digits.begin(), _digits.begin() + removeDigits);
  if (bitsToShift == 0) {
    return *this;
  }

  const auto shiftPos  = u32(bitsPerDigit - bitsToShift);
  const auto lowerMask = numeric_limits<u32>::max() >> shiftPos;
  CONTRACT_ASSERT(shiftPos < bitsPerDigit);

  if (_digits.size() == 1U) {
    _digits[0] >>= u32(bitsToShift);
    _normalize();
    return *this;
  }

  CONTRACT_ASSERT(_digits.size() > 1);
  for (usize i = 1; i < _digits.size(); ++i) {
    const auto bitsToPreserve = (_digits[i] & lowerMask) << shiftPos;
    _digits[i - 1] >>= u32(bitsToShift);
    _digits[i - 1] |= bitsToPreserve;
  }
  _digits.back() >>= u32(bitsToShift);
  _normalize();

  return *this;
}

bool NaturalN::isEven() const noexcept {
  if (_digits.empty()) {
    return true;
  }
  return (_digits[0] & 0x1U) == 0U;
}

void NaturalN::_normalize() {
  while (!_digits.empty() && _digits.back() == u32{0U}) {
    _digits.pop_back();
  }
}

static NaturalN largestDoubling(const NaturalN &a, NaturalN b) {
  CONTRACT_ASSERT(b != 0_U);
  while ((a - b) >= b) {
    b <<= 1;
  }
  return b;
}
pair<NaturalN, NaturalN> divmod(NaturalN dividend, const NaturalN &divisor) {
  if (divisor == 0_U) {
    throw invalid_argument{"division by zero is not possible"};
  }
  if (dividend == 0_U) {
    return {0_U, 0_U};
  }
  if (dividend < divisor) {
    return {0_U, dividend};
  }
  auto helper   = largestDoubling(dividend, divisor);
  auto quotient = 1_U;
  dividend -= helper;
  while (helper != divisor) {
    helper >>= 1;
    quotient <<= 1;

    if (helper <= dividend) {
      dividend -= helper;
      quotient += 1_U;
    }
  }
  return {quotient, dividend};
}

template <u8 Base> string writeInBase(NaturalN n) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");

  string reverseDigits;
  const auto base = NaturalN{Base};
  while (n > 0_U) {
    auto [quotient, remainder] = divmod(n, base);
    n                          = move(quotient);
    reverseDigits += digitToChar<Base>(remainder.convertTo<u8>());
  }

  reverse(reverseDigits.begin(), reverseDigits.end());
  return reverseDigits;
}

ostream &operator<<(ostream &os, NaturalN n) {
  const auto flags = os.flags();
  if ((flags & ios_base::dec) != 0) {
    os << writeInBase<10>(move(n));
  } else if ((flags & ios_base::oct) != 0) {
    if ((flags & ios_base::showbase) != 0) {
      os << "0";
    }
    os << writeInBase<8>(move(n));
  } else if ((flags & ios_base::hex) != 0) {
    if ((flags & ios_base::showbase) != 0) {
      os << "0x";
    }
    os << writeInBase<16>(move(n));
  } else {
    CONTRACT_ASSERT(false && "Either number base must be configured");
  }
  return os;
}

template <u8 Base>
NaturalN interpretDigitsInBaseNaturalN(const vector<u8> &digitsHighestFirst) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");
  const auto base = NaturalN{Base};
  auto result     = 0_U;
  auto position   = 1_U;
  for (u8 digit : ranges::reverse_view(digitsHighestFirst)) {
    result += position * NaturalN{digit};
    position *= base;
  }

  return result;
}

template <u8 Base> NaturalN extractNumberAsNaturalN(istream &is) {
  vector<u8> digitsHighestFirst;
  // Read from the stream as long as the next character is a digit.
  // Extracts each digit into 'digitsHighestFirst'.
  while (auto digit = nextDigit<Base>(is)) {
    digitsHighestFirst.emplace_back(digit.value());
  }
  return interpretDigitsInBaseNaturalN<Base>(digitsHighestFirst);
}

istream &operator>>(istream &is, NaturalN &n) {
  const auto flags = is.flags();
  if ((flags & ios_base::dec) != 0) {
    n = extractNumberAsNaturalN<10>(is);
  } else if ((flags & ios_base::oct) != 0) {
    n = extractNumberAsNaturalN<8>(is);
  } else if ((flags & ios_base::hex) != 0) {
    n = extractNumberAsNaturalN<16>(is);
  }
  return is;
}

NaturalN operator""_U(const char *literal, size_t /*len*/) {
  auto r  = 0_U;
  auto ss = stringstream{literal};
  ss >> r;
  return r;
}
} // namespace jt::math
