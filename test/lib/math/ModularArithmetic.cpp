#include "jt-computing/math/ModularArithmetic.hpp"
#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/Concepts.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

TEST_CASE("Modular Addition", "") {
  SECTION("small than mod") {
    REQUIRE(plus_mod{57_N}(20_N, 15_N) == 35_N);
    REQUIRE(plus_mod{17_N}(0_N, 15_N) == 15_N);
    REQUIRE(plus_mod{10_N}(5_N, 3_N) == 8_N);
  }

  SECTION("equal mod") {
    REQUIRE(plus_mod{57_N}(20_N, 37_N) == 0_N);
    REQUIRE(plus_mod{17_N}(0_N, 17_N) == 0_N);
    REQUIRE(plus_mod{10_N}(9_N, 1_N) == 0_N);
  }

  SECTION("bigger than mod") {
    REQUIRE(plus_mod{57_N}(30_N, 37_N) == 10_N);
    REQUIRE(plus_mod{17_N}(1239_N, 17_N) == 15_N);
    REQUIRE(plus_mod{10_N}(9_N, 21_N) == 0_N);
  }
}
TEST_CASE("Modular Multiplication", "") {
  SECTION("small than mod") {
    REQUIRE(multiplies_mod{57_N}(2_N, 21_N) == 42_N);
    REQUIRE(multiplies_mod{17_N}(1_N, 15_N) == 15_N);
    REQUIRE(multiplies_mod{10_N}(3_N, 3_N) == 9_N);
  }

  SECTION("equal mod") {
    REQUIRE(multiplies_mod{64_N}(8_N, 8_N) == 0_N);
    REQUIRE(multiplies_mod{17_N}(1_N, 17_N) == 0_N);
    REQUIRE(multiplies_mod{10_N}(5_N, 2_N) == 0_N);
  }

  SECTION("bigger than mod") {
    REQUIRE(multiplies_mod{57_N}(30_N, 37_N) == 27_N);
    REQUIRE(multiplies_mod{17_N}(1239_N, 31_N) == 6_N);
    REQUIRE(multiplies_mod{10_N}(9_N, 2_N) == 8_N);
  }
}
