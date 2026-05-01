module;

#include "jt-computing/core/Contracts.hpp"

export module jt.Math:NaturalN;

import std;
import jt.Core;

using namespace std;

export namespace jt::math {

/// Represents an arbitrary natural number, using @c BigUInt logic with builtin
/// interger types instead of individual bits.
class NaturalN {
public:
  NaturalN() { _digits.reserve(8); }

  /// Construct the number from a builtin unsigned integer @c value.
  explicit NaturalN(unsigned_integral auto value);

  NaturalN(const NaturalN &other)            = default;
  NaturalN(NaturalN &&other)                 = default;

  NaturalN &operator=(const NaturalN &other) = default;
  NaturalN &operator=(NaturalN &&other)      = default;

  ~NaturalN()                                = default;

  bool operator==(const NaturalN &other) const noexcept;
  strong_ordering operator<=>(const NaturalN &other) const noexcept;

  NaturalN &operator+=(const NaturalN &other);
  NaturalN &operator-=(const NaturalN &other);
  NaturalN &operator*=(const NaturalN &other);
  NaturalN &operator/=(const NaturalN &other);
  NaturalN &operator%=(const NaturalN &other);
  NaturalN &operator<<=(int value) PRE(value >= 0)
      POST(_digits.empty() || _digits.back() != 0U);
  NaturalN &operator>>=(int value) PRE(value >= 0);

  [[nodiscard]] bool isEven() const noexcept;
  [[nodiscard]] bool isOdd() const noexcept { return !isEven(); }

  /// Tries to convert to a builtin type. If the value does not fit, it throws
  /// an 'out_of_range' exception instead of performing a narrowing
  /// conversion.
  template <unsigned_integral Target> Target convertTo() const;

private:
  void _normalize();

  vector<u32> _digits;
  constexpr static int bitsPerDigit{8 * sizeof(u32)};
};

/// Represents an arbitrary natural number, using @c BigUInt logic with builtin
/// interger types instead of individual bits.
NaturalN::NaturalN(unsigned_integral auto value) {
  if (value > numeric_limits<u64>::max()) {
    throw invalid_argument{"Maximal u64::max allowed for int constructor"};
  }
  _digits.reserve(32);
  u64 v{value};
  while (v > 0U) {
    _digits.emplace_back(static_cast<u32>(v));
    v >>= unsigned{bitsPerDigit};
  }
}

pair<NaturalN, NaturalN> divmod(NaturalN dividend, const NaturalN &divisor);

NaturalN operator""_U(unsigned long long literal) { return NaturalN{literal}; }
NaturalN operator""_U(char const *literal, size_t len);

NaturalN operator+(NaturalN a, const NaturalN &b) { return a += b; }
NaturalN operator-(NaturalN a, const NaturalN &b) { return a -= b; }
NaturalN operator*(NaturalN a, const NaturalN &b) { return a *= b; }
NaturalN operator/(NaturalN a, const NaturalN &b) { return a /= b; }
NaturalN operator%(NaturalN a, const NaturalN &b) { return a %= b; }

/// Write 'n' to 'os', optionally adhering to the base modifiers.
ostream &operator<<(ostream &os, NaturalN n)
    PRE((os.flags() & (ios_base::dec | ios_base::oct | ios_base::hex)) != 0);

/// Parse 'n' from 'is', optionally adhering to the base modifiers.
istream &operator>>(istream &is, NaturalN &n);

bool isEven(const NaturalN &n) noexcept { return n.isEven(); }
bool isOdd(const NaturalN &n) noexcept { return n.isOdd(); }

NaturalN identity_element(plus<NaturalN> /*op*/) { return 0_U; }
NaturalN identity_element(multiplies<NaturalN> /*op*/) { return 1_U; }

} // namespace jt::math
