#include "jt-computing/math/NaturalN.hpp"
#include "jt-computing/math/GenericPower.hpp"

#include <algorithm>
#include <bit>
#include <cassert>
#include <compare>
#include <iostream>
#include <optional>
#include <ranges>
#include <sstream>
#include <stdexcept>

namespace jt::math {
bool NaturalN::operator==(const NaturalN &other) const noexcept {
  return (*this <=> other) == std::strong_ordering::equal;
}

std::strong_ordering
NaturalN::operator<=>(const NaturalN &other) const noexcept {
  auto digitsComparison = _digits.size() <=> other._digits.size();
  // - This number has less digits => this number must be smaller than other.
  // - This number has more digits => this number must be bigger than other.
  if (digitsComparison != std::strong_ordering::equal) {
    return digitsComparison;
  }

  assert(_digits.size() == other._digits.size());

  if (_digits.empty()) {
    return std::strong_ordering::equal;
  }

  for (usize i = _digits.size(); i > 0; --i) {
    const auto idx = i - 1;
    const auto &d1 = _digits[idx];
    const auto &d2 = other._digits[idx];
    if (d1 != d2) {
      return d1 < d2 ? std::strong_ordering::less
                     : std::strong_ordering::greater;
    }
  }

  // All digits are identical, so this number is not smaller than @c other.
  return std::strong_ordering::equal;
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
  if (magnitudeRelation == std::strong_ordering::less) {
    throw std::domain_error{"unsigned-subtraction would yield negative result"};
  }

  // Both number are equal. The result of subtraction is the neutral element, @c
  // 0U.
  if (magnitudeRelation == std::strong_ordering::equal) {
    *this = NaturalN{0U};
    return *this;
  }

  // A number with length 0 is the neutral element @c 0U and will not change
  // this number.
  if (other._digits.size() == usize{0U}) {
    return *this;
  }

  assert(magnitudeRelation == std::strong_ordering::greater);

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
    return *this = power_monoid(*this, other, std::plus<NaturalN>{});
#else
  assert(!_digits.empty());
  assert(!other._digits.empty());

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
  assert(value >= 0);

  const auto newDigits   = value / bitsPerDigit;
  const auto bitsToShift = value % bitsPerDigit;

  if (newDigits > 0) {
    _digits.insert(_digits.begin(), static_cast<usize>(newDigits), 0U);
  }
  if (bitsToShift == 0 || _digits.empty()) {
    return *this;
  }

  const auto shiftPos  = u32(bitsPerDigit - bitsToShift);
  const auto upperMask = std::numeric_limits<u32>::max() << shiftPos;

  if (std::countl_zero(_digits.back()) < bitsToShift) {
    _digits.emplace_back(0U);
  }

  auto carryOver = u32{0U};
  for (auto &digit : _digits) {
    const auto nextCarryOver = (digit & upperMask) >> shiftPos;
    digit <<= u32(bitsToShift);
    digit |= carryOver;
    carryOver = nextCarryOver;
  }

  assert(_digits.back() != 0U &&
         "Inserting a zero digit is only done if necessary");
  return *this;
}
NaturalN &NaturalN::operator>>=(int value) {
  assert(value >= 0);

  const auto removeDigits = value / bitsPerDigit;
  const auto bitsToShift  = value % bitsPerDigit;

  if (usize(removeDigits) >= _digits.size()) {
    _digits.clear();
    return *this;
  }

  assert(usize(removeDigits) < _digits.size());
  _digits.erase(_digits.begin(), _digits.begin() + removeDigits);
  if (bitsToShift == 0) {
    return *this;
  }

  const auto shiftPos  = u32(bitsPerDigit - bitsToShift);
  const auto lowerMask = std::numeric_limits<u32>::max() >> shiftPos;
  assert(shiftPos < bitsPerDigit);

  if (_digits.size() == 1U) {
    _digits[0] >>= u32(bitsToShift);
    _normalize();
    return *this;
  }

  assert(_digits.size() > 1);
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

std::size_t NaturalN::trailingZeroDigits() const {
  auto bits = std::size_t{0U};

  for (auto const &d : _digits) {
    if (d == 0U) {
      bits += 32U;
    } else {
      return bits += static_cast<std::size_t>(std::countr_zero(d));
    }
  }
  std::unreachable();
  return bits;
}

void NaturalN::_normalize() {
  while (!_digits.empty() && _digits.back() == u32{0U}) {
    _digits.pop_back();
  }
}

static NaturalN largestDoubling(const NaturalN &a, NaturalN b) {
  assert(b != 0_U);
  while ((a - b) >= b) {
    b <<= 1;
  }
  return b;
}
std::pair<NaturalN, NaturalN> divmod(NaturalN dividend,
                                     const NaturalN &divisor) {
  if (divisor == 0_U) {
    throw std::invalid_argument{"division by zero is not possible"};
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

NaturalN operator""_U(unsigned long long literal) { return NaturalN{literal}; }
NaturalN operator""_U(const char *literal, std::size_t /*len*/) {
  auto r  = 0_U;
  auto ss = std::stringstream{literal};
  ss >> r;
  return r;
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

template <u8 Base> std::string writeInBase(NaturalN n) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");

  std::string reverseDigits;
  const auto base = NaturalN{Base};
  while (n > 0_U) {
    auto [quotient, remainder] = divmod(n, base);
    n                          = std::move(quotient);
    reverseDigits += digitToChar<Base>(remainder.convertTo<u8>());
  }

  std::reverse(reverseDigits.begin(), reverseDigits.end());
  return reverseDigits;
}

std::ostream &operator<<(std::ostream &os, NaturalN n) {
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
NaturalN interpretDigitsInBase(const std::vector<u8> &digitsHighestFirst) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16,
                "Only the common number bases, either power of 2 or base 10 "
                "are supported");
  const auto base = NaturalN{Base};
  auto result     = 0_U;
  auto position   = 1_U;
  for (u8 digit : std::ranges::reverse_view(digitsHighestFirst)) {
    result += position * NaturalN{digit};
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

template <u8 Base> NaturalN extractNumber(std::istream &is) {
  std::vector<u8> digitsHighestFirst;
  // Read from the stream as long as the next character is a digit.
  // Extracts each digit into 'digitsHighestFirst'.
  while (auto digit = nextDigit<Base>(is)) {
    digitsHighestFirst.emplace_back(digit.value());
  }
  return interpretDigitsInBase<Base>(digitsHighestFirst);
}

std::istream &operator>>(std::istream &is, NaturalN &n) {
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
} // namespace jt::math
