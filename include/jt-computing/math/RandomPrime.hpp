#pragma once

#include "jt-computing/Types.hpp"
#include "jt-computing/math/NaturalN.hpp"

namespace jt::math {
/// Generates a random number of @p bits length. The highest order bit is
/// guaranteed to be set.
/// @pre bits > 0
/// @post result.size() == bits
NaturalN generateRandomNumber(i32 bits);

/// Generates a random number that is smaller than @p threshold.
/// @post results.binaryDigits() <= threshold.binaryDigits()
NaturalN randomNumberSmallerThan(NaturalN const &threshold);

/// Searches for the next prime number starting from @p startingPoint.
/// If @p startingPoint is even, one is added first.
/// Then every consecutive odd number is checked:
/// - is it divisible by a known list of the first prime numbers?
/// - if it is not obviously compound, perform Miller-Rabin-Test
/// @return the first prime number after @p startingPoint according to
///         Miller-Rabin-Test.
NaturalN findNextPrime(NaturalN startingPoint);

/// Performs the probabilistic Miller-Rabin Primality Tests for @p number.
bool isLikelyPrime(NaturalN const& number);
} // namespace jt::math
