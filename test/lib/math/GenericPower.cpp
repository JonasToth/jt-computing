#include "jt-computing/math/GenericPower.hpp"
#include "jt-computing/math/AlgebraConcepts.hpp"
#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/FixedSquareMatrix.hpp"
#include "jt-computing/math/ModularArithmetic.hpp"

#include <chrono>
#include <initializer_list>
#include <iomanip>
#include <memory>
#include <random>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

template <>
std::string jt::math::identity_element(std::plus<std::string> /*op*/) {
  return "";
}

TEST_CASE("GenericPower", "") {
  SECTION("Integer Multiplication") {
    REQUIRE(power_monoid(10, 2, std::plus<int>{}) == 20);
    REQUIRE(power_monoid(i64{1203}, i64{12}, std::plus<i64>{}) == i64{14436});
    REQUIRE(power_monoid(u64{1203}, u64{12}, std::plus<u64>{}) == i64{14436});
  }
  SECTION("Integer Exponentiation") {
    REQUIRE(power_monoid(i64{10}, i64{5}) == i64{100000});
  }
  SECTION("BigUInt Exponentiation") {
    REQUIRE(power_semigroup(10_N, 2_N) == 100_N);
    REQUIRE(power_monoid(10_N, 2_N) == 100_N);
    REQUIRE(power_monoid(10_N, 2) == 100_N);
    REQUIRE(power_monoid(10_N, 4_N) == 10000_N);
    REQUIRE(power_monoid(9_N, 15) == 205891132094649_N);
  }
  SECTION("Modulo Exponentiation") {
    REQUIRE(power_monoid(7_N, 23_N, multiplies_mod{143_N}) == 2_N);
    REQUIRE(power_monoid(2_N, 1721_N, multiplies_mod{263713_N}) == 73047_N);
    REQUIRE(power_monoid(1234_N, 1721_N, multiplies_mod{263713_N}) == 157495_N);

    const auto plain  = 230911_N;

    const auto modul  = 263713_N;
    const auto e      = 1721_N;
    const auto d      = 1373_N;
    const auto cipher = power_semigroup(plain, e, multiplies_mod{modul});
    REQUIRE(cipher == 1715_N);

    const auto decipher = power_semigroup(cipher, d, multiplies_mod{modul});
    REQUIRE(decipher == plain);
  }
  SECTION("Raising String To Power") {
    REQUIRE(
        power_semigroup(std::string{"Hello"}, 8, std::plus<std::string>{}) ==
        std::string{"HelloHelloHelloHelloHelloHelloHelloHello"});
    REQUIRE(power_monoid(std::string{"Hello"}, 0, std::plus<std::string>{}) ==
            std::string{""});
  }
}

TEST_CASE("PowerSquareMatrix", "") {
  SECTION("Small Fibbonacci Number i64") {
    const auto fibbonacciMatrix = FixedSquareMatrix<i64, 2>{1, 1, 1, 0};
    const auto pow              = power_monoid(fibbonacciMatrix, 10);
    const auto result           = FixedSquareMatrix<i64, 2>{89, 55, 55, 34};
    REQUIRE(pow == result);
  }
  SECTION("Massive Fibbonacci Number BigUInt") {
    const auto fibbonacciMatrix =
        FixedSquareMatrix<BigUInt, 2>{1_N, 1_N, 1_N, 0_N};
    const auto pow    = power_monoid(fibbonacciMatrix, 1000);
    const auto result = FixedSquareMatrix<BigUInt, 2>{
        "703303677114228158218352548771835497701812698363587"
        "327426049050871545371181969335797422494945626117334"
        "877504492417659910881863632654502236471060120533741"
        "212738673391111981393731255987676900919022452453234"
        "03501"_N,
        "434665576869374564356885276750406258025646605173717"
        "804024817290895365554179490518904038798400792551692"
        "959225930803226347752096896232398733224711616429964"
        "409065331879382989696499285160037044761377951668492"
        "28875"_N,
        "434665576869374564356885276750406258025646605173717"
        "804024817290895365554179490518904038798400792551692"
        "959225930803226347752096896232398733224711616429964"
        "409065331879382989696499285160037044761377951668492"
        "28875"_N,
        "268638100244853593861467272021429239676166093189869"
        "523401231759976179817002478816893383696544833565641"
        "918278561614433563129766736422103503246348504103776"
        "803673341511728991697231970827639856157644500784741"
        "74626"_N};
    REQUIRE(pow == result);
  }
}
