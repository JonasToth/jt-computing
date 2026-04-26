module jt.Math:Helpers;

import std;
import jt.Core;

namespace jt::math {

/// Convert a digit of any @c Base to a single character for textual output operations.
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

  // assert(false && "Unreachable");
}

/// Try to retrieve the next digit in @c Base from the input stream @c is.
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
}
