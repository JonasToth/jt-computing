module;

#include "jt-computing/core/Contracts.hpp"

export module jt.Math:Operations;

import std;

export namespace jt::math {

/// Performs division with remainder as single function call.
template <std::unsigned_integral NaturalNumber>
std::pair<NaturalNumber, NaturalNumber> divmod(NaturalNumber dividend,
                                               NaturalNumber divisor)
    PRE(divisor != NaturalNumber{0U}) {
  return {dividend / divisor, dividend % divisor};
}

} // namespace jt::math
