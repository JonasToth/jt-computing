module;

#include "jt-computing/core/Contracts.hpp"

module jt.Math:BigUInt.Impl;

import :BigUInt;
import :NumberIO;

import std;
import jt.Core;

namespace jt::math {

template <u8 Base>
BigUInt
interpretDigitsInBaseBigUInt(const std::vector<u8> &digitsHighestFirst) {
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

template <u8 Base> BigUInt extractNumberAsBigUInt(std::istream &is) {
  std::vector<u8> digitsHighestFirst;
  // Read from the stream as long as the next character is a digit.
  // Extracts each digit into 'digitsHighestFirst'.
  while (auto digit = nextDigit<Base>(is)) {
    digitsHighestFirst.emplace_back(digit.value());
  }
  return interpretDigitsInBaseBigUInt<Base>(digitsHighestFirst);
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
    CONTRACT_ASSERT(false && "Either number base must be configured");
  }
  return os;
}

std::istream &operator>>(std::istream &is, BigUInt &n) {
  const auto flags = is.flags();
  if ((flags & std::ios_base::dec) != 0) {
    n = extractNumberAsBigUInt<10>(is);
  } else if ((flags & std::ios_base::oct) != 0) {
    n = extractNumberAsBigUInt<8>(is);
  } else if ((flags & std::ios_base::hex) != 0) {
    n = extractNumberAsBigUInt<16>(is);
  }
  return is;
}
}
