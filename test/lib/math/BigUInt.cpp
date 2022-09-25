#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/Concepts.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

// Verify that the concept for natural numbers works.
static_assert(Computable<BigUInt>);
static_assert(NaturalNumber<BigUInt>);
static_assert(NaturalNumber<unsigned int>);
static_assert(!NaturalNumber<int>);

TEST_CASE("BigUInt Construction", "") {
  SECTION("2^11") {
    BigUInt bu{u64{2048ULL}};
    REQUIRE(bu.binaryDigits() == 12);
  }

  SECTION("Random Number below 2^11") {
    BigUInt bu{u64{1412ULL}};
    REQUIRE(bu.binaryDigits() == 11);
  }

  SECTION("0 must be empty") {
    BigUInt bu{u64{0ULL}};
    REQUIRE(bu.binaryDigits() == 0);
  }
}

TEST_CASE("BigUInt ConversionToBuiltin", "") {
  SECTION("u8 fits") {
    const auto N = 255_N;
    const auto C = N.convertTo<u8>();
    REQUIRE(C == 255U);
  }
  SECTION("u8 wouldNarrow") {
    const auto N = 256_N;
    REQUIRE_THROWS_AS(N.convertTo<u8>(), std::out_of_range);

    const auto C = N.convertTo<u16>();
    REQUIRE(C == 256U);
  }

  SECTION("usize random") {
    const auto N = 12389614_N;
    const auto C = N.convertTo<usize>();
    REQUIRE(C == 12389614ULL);
  }
}

TEST_CASE("BigUInt Comparison", "") {
  SECTION("With other BigUInt") {
    BigUInt a{u32{1249U}};
    BigUInt b{u32{1234U}};
    BigUInt c{u32{1249U}};

    REQUIRE(a == a);
    REQUIRE(a != b);
    REQUIRE(b != c);
    REQUIRE(b == b);
    REQUIRE(a == c);
  }

  SECTION("With builtin unsigned") {
    BigUInt a{u32{1249U}};

    REQUIRE(a == 1249U);
    REQUIRE(a == u64{1249UL});
    REQUIRE(a != 124U);
    REQUIRE(a != u64{124UL});
  }
}

TEST_CASE("BigUInt <=> relation", "") {
  SECTION("With other BigUInt") {
    BigUInt a{u32{1249U}};
    BigUInt b{u32{1234U}};
    BigUInt c{u32{1249U}};
    BigUInt d{u32{5U}};
    BigUInt e{u32{7U}};

    REQUIRE_FALSE(a < a);
    REQUIRE_FALSE(a < b);
    REQUIRE(b < c);
    REQUIRE_FALSE(b < b);
    REQUIRE_FALSE(a < c);

    REQUIRE(d < e);
    REQUIRE_FALSE(e < d);
  }

  SECTION("With builtin unsigned") {
    BigUInt a{u32{1249U}};

    REQUIRE_FALSE(a < 1249U);
    REQUIRE(a < u64{12492UL});
    REQUIRE_FALSE(a < 124U);
    REQUIRE(a < u64{1250UL});

    BigUInt b{u32{5U}};
    REQUIRE(b < 7U);
    REQUIRE_FALSE(BigUInt{7U} < 5U);
    REQUIRE(3U < b);
  }

  SECTION("<= >= >") {
    BigUInt a{2341U};

    REQUIRE(a <= 2341U);
    REQUIRE(a > 2340U);
    REQUIRE(a >= 2341U);
    REQUIRE(2341U > BigUInt{23U});
  }
}

TEST_CASE("BigUInt Addition", "") {
  SECTION("0+0") {
    BigUInt a{0U};
    a += BigUInt{0U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("0+1") {
    BigUInt a{0U};
    a += BigUInt{1U};
    REQUIRE(a == BigUInt{1U});
  }

  SECTION("1+1") {
    BigUInt a{1U};
    a += BigUInt{1U};
    REQUIRE(a == BigUInt{2U});
  }

  SECTION("1+2") {
    BigUInt a{1U};
    a += BigUInt{2U};
    REQUIRE(a == BigUInt{3U});
  }

  SECTION("2+1") {
    BigUInt a{2U};
    a += BigUInt{1U};
    REQUIRE(a == BigUInt{3U});
  }

  SECTION("3+1") {
    BigUInt a{3U};
    a += BigUInt{1U};
    REQUIRE(a == BigUInt{4U});
  }

  SECTION("3+2") {
    BigUInt a{3U};
    a += BigUInt{2U};
    REQUIRE(a == BigUInt{5U});
  }

  SECTION("3+3") {
    BigUInt a{3U};
    a += BigUInt{3U};
    REQUIRE(a == BigUInt{6U});
  }

  SECTION("3+4") {
    BigUInt a{3U};
    BigUInt b{4U};
    a += b;
    REQUIRE(a == BigUInt{7U});
  }

  SECTION("3+5") {
    BigUInt a{3U};
    BigUInt b{5U};
    a += b;
    REQUIRE(a == BigUInt{8U});
  }

  SECTION("3+6") {
    BigUInt a{3U};
    BigUInt b{6U};
    a += b;
    REQUIRE(a == BigUInt{9U});
  }

  SECTION("4+1") {
    BigUInt a{4U};
    a += BigUInt{1U};
    REQUIRE(a == BigUInt{5U});
  }

  SECTION("1+10") {
    BigUInt a{1U};
    a += BigUInt{10U};
    REQUIRE(a == BigUInt{11U});
  }

  SECTION("Arbitrary Number + Arbitrary Number") {
    BigUInt a{41983U};
    BigUInt b{2958912349U};

    a += b;
    REQUIRE(a == BigUInt{41983U + 2958912349U});
  }
}

TEST_CASE("BigUInt Subtraction", "") {
  SECTION("0-0") {
    BigUInt a{0U};
    a -= BigUInt{0U};
    REQUIRE(a == BigUInt{0U});
  }

  SECTION("a - a == 0") {
    BigUInt a{12381923U};
    BigUInt b{a};

    a -= b;
    REQUIRE(a == 0U);
  }

  SECTION("6 - 5 == 1") {
    BigUInt a{6U};
    BigUInt b{5U};
    a -= b;
    REQUIRE(a == 1U);
  }

  SECTION("6 - 1 == 5") {
    BigUInt a{6U};
    BigUInt b{1U};
    a -= b;
    REQUIRE(a == 5U);
  }

  SECTION("2 - 1 == 1") {
    BigUInt a{2U};
    BigUInt b{1U};
    a -= b;
    REQUIRE(a == 1U);
  }

  SECTION("10 - 3 == 7") {
    BigUInt a{10U};
    BigUInt b{3U};

    a -= b;
    REQUIRE(a == 7U);
  }

  SECTION("8 - 1 == 7") {
    BigUInt a{8U};
    BigUInt b{1U};
    a -= b;
    REQUIRE(a == 7U);
  }

  SECTION("32 - 16 == 16") {
    BigUInt a{32U};
    BigUInt b{16U};
    a -= b;
    REQUIRE(a == 16U);
  }

  SECTION("31 - 16 == 15") {
    BigUInt a{31U};
    BigUInt b{16U};
    a -= b;
    REQUIRE(a == 15U);
  }

  SECTION("1039812U - 1239U is the same builtin result") {
    BigUInt a{1039812U};
    BigUInt b{1239U};
    a -= b;
    REQUIRE(a == 1039812U - 1239U);
  }

  SECTION("0 - 1 must throw a domain_error") {
    BigUInt a{0U};
    BigUInt b{1U};
    REQUIRE_THROWS_AS(a -= b, std::domain_error);
  }
  SECTION("12310914U - 112839149123U must throw a domain_error") {
    BigUInt a{12310914U};
    BigUInt b{112839149123U};
    REQUIRE_THROWS_AS(a -= b, std::domain_error);
  }
}

TEST_CASE("Multiplication", "") {
  SECTION("0 x 0") {
    BigUInt a{0U};
    a *= BigUInt{0U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("0 x 10") {
    BigUInt a{0U};
    a *= BigUInt{10U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("1 x 10") {
    BigUInt a{1U};
    a *= BigUInt{10U};
    REQUIRE(a == BigUInt{10U});
  }
  SECTION("10 x 1") {
    BigUInt a{10U};
    a *= BigUInt{1U};
    REQUIRE(a == BigUInt{10U});
  }
  SECTION("10 x 42") {
    BigUInt a{10U};
    a *= BigUInt{42U};
    REQUIRE(a == BigUInt{420U});
  }
  SECTION("11 x 42") {
    BigUInt a{11U};
    a *= BigUInt{42U};
    REQUIRE(a == BigUInt{462U});
  }
  SECTION("11 x 53") {
    BigUInt a{11U};
    a *= BigUInt{53U};
    REQUIRE(a == BigUInt{11U * 53U});
  }

  SECTION("1283912381092 x 564123") {
    BigUInt a{1283912381092ULL};
    a *= BigUInt{564123ULL};
    REQUIRE(a == BigUInt{1283912381092ULL * 564123ULL});
  }
}

TEST_CASE("Division", "") {
  SECTION("0 / 0") {
    BigUInt a{0U};
    REQUIRE_THROWS_AS(a /= BigUInt{0U}, std::invalid_argument);
  }
  SECTION("1 / 0") {
    BigUInt a{1U};
    REQUIRE_THROWS_AS(a /= BigUInt{0U}, std::invalid_argument);
  }
  SECTION("0 / 10") {
    BigUInt a{0U};
    a /= BigUInt{10U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("1 / 10") {
    BigUInt a{1U};
    a /= BigUInt{10U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("10 / 1") {
    BigUInt a{10U};
    a /= BigUInt{1U};
    REQUIRE(a == BigUInt{10U});
  }
  SECTION("42 / 10") {
    BigUInt a{42U};
    a /= BigUInt{10U};
    REQUIRE(a == BigUInt{4U});
  }
  SECTION("1872 / 15") {
    BigUInt a{1872U};
    a /= 15U;
    const auto [q, r] = divmod(a, BigUInt{15U});
    REQUIRE(a == 15U * q + r);
    REQUIRE(a == 124U);
  }

  SECTION("1283912381092 / 564123") {
    BigUInt a{1283912381092ULL};
    a /= BigUInt{564123ULL};
    REQUIRE(a == BigUInt{1283912381092ULL / 564123ULL});
  }
}

TEST_CASE("Modulus", "") {
  SECTION("0 % 0") {
    BigUInt a{0U};
    REQUIRE_THROWS_AS(a %= BigUInt{0U}, std::invalid_argument);
  }
  SECTION("1 % 0") {
    BigUInt a{1U};
    REQUIRE_THROWS_AS(a %= BigUInt{0U}, std::invalid_argument);
  }
  SECTION("0 % 10") {
    BigUInt a{0U};
    a %= BigUInt{10U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("1 % 10") {
    BigUInt a{1U};
    a %= BigUInt{10U};
    REQUIRE(a == BigUInt{1U});
  }
  SECTION("10 % 1") {
    BigUInt a{10U};
    a %= BigUInt{1U};
    REQUIRE(a == BigUInt{0U});
  }
  SECTION("42 % 10") {
    BigUInt a{42U};
    a %= BigUInt{10U};
    REQUIRE(a == BigUInt{2U});
  }

  SECTION("1283912381092 % 564123") {
    BigUInt a{1283912381092ULL};
    a %= BigUInt{564123ULL};
    REQUIRE(a == BigUInt{1283912381092ULL % 564123ULL});
  }
}

TEST_CASE("BigUInt LeftShift", "") {
  SECTION("Shift 0 by 1 remains 0") {
    BigUInt a{0U};
    a <<= 1;
    REQUIRE(a == BigUInt{0U});
    REQUIRE(a.binaryDigits() == usize{0U});
  }
  SECTION("Shift 1 by 1 becomes 2") {
    BigUInt a{1U};
    a <<= 1;
    REQUIRE(a == BigUInt{2U});
    REQUIRE(a.binaryDigits() == usize{2U});
  }
  SECTION("Shift 1 by 20 becomes 2^20") {
    BigUInt a{1U};
    a <<= 20;
    REQUIRE(a == BigUInt{1U << 20U});
    REQUIRE(a.binaryDigits() == usize{21U});
  }
  SECTION("Shift weird number a couple of times equals doubing") {
    BigUInt a{11235123U};
    a <<= 4;
    REQUIRE(a == BigUInt{11235123U * 2U * 2U * 2U * 2U});
    REQUIRE(a.binaryDigits() == usize{28U});
  }
}

TEST_CASE("BigUInt RightShift", "") {
  SECTION("Shift 0 by 1 remains 0") {
    BigUInt a{0U};
    a >>= 1;
    REQUIRE(a == BigUInt{0U});
    REQUIRE(a.binaryDigits() == usize{0U});
  }
  SECTION("Shift 1 by 1 becomes 0") {
    BigUInt a{1U};
    a >>= 1;
    REQUIRE(a == BigUInt{0U});
    REQUIRE(a.binaryDigits() == usize{0U});
  }
  SECTION("Shift 2^20 by 20 becomes 1") {
    BigUInt a{1U << 20U};
    a >>= 20;
    REQUIRE(a == BigUInt{1U});
    REQUIRE(a.binaryDigits() == usize{1U});
  }
  SECTION("Shift weird number a couple of times equals halfing") {
    BigUInt a{11235123U};
    a >>= 4;
    REQUIRE(a == BigUInt{11235123U / 2U / 2U / 2U / 2U});
    REQUIRE(a.binaryDigits() == usize{20U});
  }
}

TEST_CASE("OddityCheck", "") {
  SECTION("0 is even") {
    BigUInt a{0U};
    REQUIRE(a.isEven());
    REQUIRE(!a.isOdd());
  }
  SECTION("2 is even") {
    BigUInt a{2U};
    REQUIRE(a.isEven());
    REQUIRE(!a.isOdd());
  }
  SECTION("1602134 is even") {
    BigUInt a{1602134U};
    REQUIRE(a.isEven());
    REQUIRE(!a.isOdd());
  }
  SECTION("1 is uneven") {
    BigUInt a{1U};
    REQUIRE(!a.isEven());
    REQUIRE(a.isOdd());
  }
  SECTION("15 is uneven") {
    BigUInt a{15U};
    REQUIRE(!a.isEven());
    REQUIRE(a.isOdd());
  }
  SECTION("1231241239123141 is uneven") {
    BigUInt a{1231241239123141U};
    REQUIRE(!a.isEven());
    REQUIRE(a.isOdd());
  }
}

TEST_CASE("DivMod", "") {
  SECTION("division by 0 throws exception") {
    BigUInt a{10U};
    BigUInt b{0U};
    REQUIRE_THROWS_AS(divmod(a, b), std::invalid_argument);
  }
  SECTION("0 divided by anything is 0 with no remainder") {
    BigUInt a{10U};
    BigUInt b{0U};
    REQUIRE(divmod(b, a) == std::pair{BigUInt{0U}, BigUInt{0U}});
  }
  SECTION("<small> divided by <big> is 0, remainder <small>") {
    BigUInt a{10U};
    BigUInt b{30U};
    REQUIRE(divmod(a, b) == std::pair{BigUInt{0U}, a});
  }
  SECTION("10 / 5 == {2,0}") {
    BigUInt a{10U};
    BigUInt b{5U};
    REQUIRE(divmod(a, b) == std::pair{BigUInt{2U}, BigUInt{0U}});
  }
  SECTION("13 / 5 == {2,3}") {
    BigUInt a{13U};
    BigUInt b{5U};
    REQUIRE(divmod(a, b) == std::pair{BigUInt{2U}, BigUInt{3U}});
  }
  SECTION("1239410 / 1247 == {993,1139}") {
    BigUInt a{1239410U};
    BigUInt b{1247U};
    REQUIRE(divmod(a, b) == std::pair{BigUInt{993U}, BigUInt{1139U}});
  }
}

TEST_CASE("Literal", "") {
  const auto N = 123098124_N;
  REQUIRE(N == BigUInt{123098124U});
}
