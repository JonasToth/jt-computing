#include "jt-computing/crypto/TextbookRSA.hpp"

#include "jt-computing/math/BigInt.hpp"
#include "jt-computing/math/EuclideanAlgorithm.hpp"
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
  auto const n         = generateRandomNumber(/*bits=*/256);
  auto const nextPrime = findNextPrime(n);

  REQUIRE(nextPrime >= n);
  REQUIRE(nextPrime.isOdd());
}

TEST_CASE("Generate RSA Keys", "") {
  std::cout << "Generating New Prime Numbers\n";
  auto const p       = findNextPrime(generateRandomNumber(/*bits=*/256));
  auto const q       = findNextPrime(generateRandomNumber(/*bits=*/256));
  auto const modulus = p * q;

  REQUIRE(!isLikelyPrime(modulus));

  std::cout << "Computing RSA Key Parameters\n";
  auto const totient = euclidean_lcm(p - 1_U, q - 1_U);
  auto const e       = 65'537_U;
  auto const d       = modular_inverse(BigInt{e}, BigInt{totient});
  REQUIRE(d.has_value());

  std::cout << "Secret Key Value: " << d.value() << "\n";
  auto dd = NaturalN{d.value().abs()};

  using namespace jt::crypto;
  auto publicRSA     = TextbookRSA<Key::Public>(modulus, e);
  auto privateRSA    = TextbookRSA<Key::Private>(modulus, std::move(dd));
  auto const message = generateRandomNumber(/*bits=*/128);
  std::cout << "Performing Forward and Backward Encryption\n";
  REQUIRE(privateRSA.apply(publicRSA.apply(message)) == message);
  REQUIRE(publicRSA.apply(privateRSA.apply(message)) == message);
}
