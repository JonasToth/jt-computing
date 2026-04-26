export module jt.Math:NaturalN;

import std;
import jt.Core;

export namespace jt::math {

/// Represents an arbitrary natural number, using @c BigUInt logic with builtin
/// interger types instead of individual bits.
class NaturalN {
public:
  NaturalN() { _digits.reserve(8); }

  /// Construct the number from a builtin unsigned integer @c value.
  explicit NaturalN(std::unsigned_integral auto value);

  NaturalN(const NaturalN &other)            = default;
  NaturalN(NaturalN &&other)                 = default;

  NaturalN &operator=(const NaturalN &other) = default;
  NaturalN &operator=(NaturalN &&other)      = default;

  ~NaturalN()                                = default;

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
  constexpr static int bitsPerDigit{8 * sizeof(u32)};
};

/// Represents an arbitrary natural number, using @c BigUInt logic with builtin
/// interger types instead of individual bits.
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

std::pair<NaturalN, NaturalN> divmod(NaturalN dividend,
                                     const NaturalN &divisor);

NaturalN operator""_U(unsigned long long literal) { return NaturalN{literal}; }
NaturalN operator""_U(char const *literal, std::size_t len);

NaturalN operator+(NaturalN a, const NaturalN &b) { return a += b; }
NaturalN operator-(NaturalN a, const NaturalN &b) { return a -= b; }
NaturalN operator*(NaturalN a, const NaturalN &b) { return a *= b; }
NaturalN operator/(NaturalN a, const NaturalN &b) { return a /= b; }
NaturalN operator%(NaturalN a, const NaturalN &b) { return a %= b; }

/// Write 'n' to 'os', optionally adhering to the base modifiers.
std::ostream &operator<<(std::ostream &os, NaturalN n);

/// Parse 'n' from 'is', optionally adhering to the base modifiers.
std::istream &operator>>(std::istream &is, NaturalN &n);

bool isEven(const NaturalN &n) noexcept { return n.isEven(); }
bool isOdd(const NaturalN &n) noexcept { return n.isOdd(); }

NaturalN identity_element(std::plus<NaturalN> /*op*/) { return 0_U; }
NaturalN identity_element(std::multiplies<NaturalN> /*op*/) {
  return 1_U;
}

} // namespace jt::math
