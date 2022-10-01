#include "jt-computing/math/NaturalNumberAlgorithms.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

TEST_CASE("ComputePrimeFactors", "") {
  SECTION("0") {
    const auto factors = getPrimeFactors(0U);
    REQUIRE(factors.empty());
  }
  SECTION("1") {
    const auto factors = getPrimeFactors(1U);
    REQUIRE(factors.empty());
  }
  SECTION("2") {
    const auto factors = getPrimeFactors(2U);
    REQUIRE(factors.empty());
  }
  SECTION("3") {
    const auto factors = getPrimeFactors(3U);
    REQUIRE(factors.empty());
  }
  SECTION("4") {
    const auto factors = getPrimeFactors(4U);
    REQUIRE(factors == std::vector<unsigned int>{2U, 2U});
  }
  SECTION("5") {
    const auto factors = getPrimeFactors(5U);
    REQUIRE(factors.empty());
  }
  SECTION("6") {
    const auto factors = getPrimeFactors(6U);
    REQUIRE(factors == std::vector<unsigned int>{2U, 3U});
  }
  SECTION("8") {
    const auto factors = getPrimeFactors(8U);
    REQUIRE(factors == std::vector<unsigned int>{2U, 2U, 2U});
  }
  SECTION("15") {
    const auto factors = getPrimeFactors(15U);
    REQUIRE(factors == std::vector<unsigned int>{3U, 5U});
  }
  SECTION("120391248") {
    const auto factors = getPrimeFactors(BigUInt{120391248U});
    REQUIRE(factors == std::vector<BigUInt>{BigUInt{2U}, BigUInt{2U},
                                            BigUInt{2U}, BigUInt{2U},
                                            BigUInt{3U}, BigUInt{2508151U}});
  }
  SECTION("Prime Number Product factors into the primes again") {
    const auto factors = getPrimeFactors(BigUInt{132049U} * BigUInt{216091U});
    REQUIRE(factors ==
            std::vector<BigUInt>{BigUInt{132049U}, BigUInt{216091U}});
  }
}

TEST_CASE("IsPrime", "") {
  SECTION("First Primes") {
    REQUIRE(isPrime(0U) == false);
    REQUIRE(isPrime(1U) == false);
    REQUIRE(isPrime(2U) == true);
    REQUIRE(isPrime(5U) == true);
    REQUIRE(isPrime(13U) == true);
  }
  SECTION("Some Even numbers that are not prime") {
    REQUIRE(isPrime(8U) == false);
    REQUIRE(isPrime(1203123912U) == false);
  }
  SECTION("Uneven Non-Primes") {
    REQUIRE(isPrime(8U * 23U) == false);
    REQUIRE(isPrime(BigUInt{1223911U} * BigUInt{1230241241U}) == false);
  }
}

TEST_CASE("SieveEratosthenes", "") {
  const auto first1000 = sieveEratosthenes<BigUInt>(usize{1000U});

  REQUIRE(first1000.front() == BigUInt{2U});
  REQUIRE(first1000.back() == BigUInt{997U});
  REQUIRE(first1000.size() == usize{168U});
}

TEST_CASE("GCD", "") {
  SECTION("NaturalNumbers") {
    REQUIRE(gcd(5_N, 10_N) == 5_N);
    REQUIRE(gcd(10_N, 5_N) == 5_N);
    REQUIRE(gcd(15_N, 5_N) == 5_N);
    REQUIRE(gcd(31_N, 37_N) == 1_N);
    REQUIRE(gcd(37_N, 31_N) == 1_N);
    REQUIRE(gcd(12309182049_N, 12039812471827398123_N) == 3_N);

    REQUIRE(gcd(123'048U, 1'124U) == 4U);
  }
}

TEST_CASE("LCM", "") {
  SECTION("NaturalNumbers") {
    REQUIRE(lcm(12_N, 18_N) == 36_N);
    REQUIRE(lcm(3'528_N, 3'780_N) == 52'920_N);
  }
}
