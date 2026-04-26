export module jt.Math:Operations;

import std;

export namespace jt::math {

/// Performs division with remainder as single function call.
template <std::unsigned_integral NaturalNumber>
std::pair<NaturalNumber, NaturalNumber> divmod(NaturalNumber dividend,
                                               NaturalNumber divisor) {
  return {dividend / divisor, dividend % divisor};
}

} // namespace jt::math
