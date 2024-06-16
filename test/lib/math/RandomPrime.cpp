#include "jt-computing/math/RandomPrime.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

TEST_CASE("Small Random Int", "") {
  SECTION("sum of 2 small ints must be greater (bits+1)") {
    auto const n1  = generateRandomNumber(/*bits=*/16);
    auto const n2  = generateRandomNumber(/*bits=*/16);
    auto threshold = 1_U;
    threshold <<= 15;

    REQUIRE((n1 + n2) > threshold);
  }
}

TEST_CASE("Medium Random Prime", "") {
  auto const n         = generateRandomNumber(/*bits=*/128);
  auto const nextPrime = findNextPrime(n);

  REQUIRE(nextPrime >= n);
  REQUIRE(nextPrime.isOdd());
}

TEST_CASE("Big Random Prime", "") {
  auto const n         = generateRandomNumber(/*bits=*/1024);
  auto const nextPrime = findNextPrime(n);

  REQUIRE(nextPrime >= n);
  REQUIRE(nextPrime.isOdd());
}
