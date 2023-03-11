#pragma once

#include "jt-computing/Types.hpp"

#include <cassert>
#include <climits>
#include <compare>
#include <concepts>
#include <iosfwd>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace jt::math {

/// Represents an arbitrary natural number, using @c BigUInt logic with builtin
/// interger types instead of individual bits.
class NaturalN {
public:
  NaturalN() { _digits.reserve(8); }

  NaturalN(const NaturalN &other)            = default;
  NaturalN(NaturalN &&other)                 = default;

  NaturalN &operator=(const NaturalN &other) = default;
  NaturalN &operator=(NaturalN &&other)      = default;

  ~NaturalN()                                = default;

  /// Construct the number from a builtin unsigned integer @c value.
  explicit NaturalN(std::unsigned_integral auto value);

  bool operator==(const NaturalN &other) const noexcept;
  std::strong_ordering operator<=>(const NaturalN &other) const noexcept;

  NaturalN &operator+=(const NaturalN &other);
  NaturalN &operator-=(const NaturalN &other);
  NaturalN &operator*=(const NaturalN &other);
  NaturalN &operator/=(const NaturalN &other);
  NaturalN &operator%=(const NaturalN &other);
  NaturalN &operator<<=(int value);
  NaturalN &operator>>=(int value);

  [[nodiscard]] bool isEven() const noexcept;
  [[nodiscard]] bool isOdd() const noexcept { return !isEven(); }

  /// Tries to convert to a builtin type. If the value does not fit, it throws
  /// an 'std::out_of_range' exception instead of performing a narrowing
  /// conversion.
  template <std::unsigned_integral Target> Target convertTo() const;

private:
  void _normalize();

  std::vector<u32> _digits;
  constexpr static int bitsPerDigit{CHAR_BIT * sizeof(u32)};
};

NaturalN::NaturalN(std::unsigned_integral auto value) {
  if (value > std::numeric_limits<u64>::max()) {
    throw std::invalid_argument{"Maximal u64::max allowed for int constructor"};
  }
  _digits.reserve(32);
  u64 v{value};
  while (v > 0U) {
    _digits.emplace_back(static_cast<u32>(v));
    v >>= unsigned{bitsPerDigit};
  }
}

template <std::unsigned_integral Target> Target NaturalN::convertTo() const {
  Target result{0U};
  if (_digits.empty()) {
    return result;
  }
  assert(!_digits.empty());
  if constexpr (std::is_same_v<Target, u64>) {
    if (_digits.size() > 2) {
      throw std::out_of_range{"Conversion would narrow"};
    }
    result += !_digits.empty() ? Target{_digits[0]} : 0U;
    result += _digits.size() > 1U ? Target{_digits[1]} : 0U;
  }
  if constexpr (std::is_same_v<Target, u32>) {
    if (_digits.size() > 1) {
      throw std::out_of_range{"Conversion would narrow"};
    }
    return _digits[0];
  }
  if constexpr (std::is_same_v<Target, u16>) {
    if (_digits.size() > 1 || _digits[0] > std::numeric_limits<u16>::max()) {
      throw std::out_of_range{"Conversion would narrow"};
    }
    return Target(_digits[0]);
  }
  if constexpr (std::is_same_v<Target, u8>) {
    if (_digits.size() > 1 || _digits[0] > std::numeric_limits<u8>::max()) {
      throw std::out_of_range{"Conversion would narrow"};
    }
    return Target(_digits[0]);
  }
  assert(false && "Unreachable, because all integral types are handled");
}

std::pair<NaturalN, NaturalN> divmod(NaturalN dividend,
                                     const NaturalN &divisor);

NaturalN operator"" _U(unsigned long long literal);
NaturalN operator""_U(char const *literal, std::size_t len);

inline NaturalN operator+(NaturalN a, const NaturalN &b) { return a += b; }
inline NaturalN operator-(NaturalN a, const NaturalN &b) { return a -= b; }
inline NaturalN operator*(NaturalN a, const NaturalN &b) { return a *= b; }
inline NaturalN operator/(NaturalN a, const NaturalN &b) { return a /= b; }
inline NaturalN operator%(NaturalN a, const NaturalN &b) { return a %= b; }

/// Write 'n' to 'os', optionally adhering to the base modifiers.
std::ostream &operator<<(std::ostream &os, NaturalN n);

/// Parse 'n' from 'is', optionally adhering to the base modifiers.
std::istream &operator>>(std::istream &is, NaturalN &n);

inline bool isEven(const NaturalN &n) noexcept { return n.isEven(); }
inline bool isOdd(const NaturalN &n) noexcept { return n.isOdd(); }

inline NaturalN identity_element(std::plus<NaturalN> /*op*/) { return 0_U; }
inline NaturalN identity_element(std::multiplies<NaturalN> /*op*/) {
  return 1_U;
}

} // namespace jt::math
