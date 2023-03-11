#include "jt-computing/math/NaturalN.hpp"
#include "jt-computing/math/Concepts.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iomanip>

using namespace jt;
using namespace jt::math;

TEST_CASE("NaturalN Comparison", "") {
  SECTION("With other NaturalN") {
    NaturalN a{u32{1249U}};
    NaturalN b{u32{1234U}};
    NaturalN c{u32{1249U}};

    REQUIRE(a == a);
    REQUIRE(a != b);
    REQUIRE(b != c);
    REQUIRE(b == b);
    REQUIRE(a == c);
  }
}

TEST_CASE("NaturalN <=> relation", "") {
  SECTION("With other NaturalN") {
    NaturalN a{u32{1249U}};
    NaturalN b{u32{1234U}};
    NaturalN c{u32{1249U}};
    NaturalN d{u32{5U}};
    NaturalN e{u32{7U}};

    REQUIRE_FALSE(a < a);
    REQUIRE_FALSE(a < b);
    REQUIRE(b < c);
    REQUIRE_FALSE(b < b);
    REQUIRE_FALSE(a < c);

    REQUIRE(d < e);
    REQUIRE_FALSE(e < d);
  }
}

TEST_CASE("NaturalN Addition", "") {
  SECTION("0+0") {
    NaturalN a{0U};
    a += NaturalN{0U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("0+1") {
    NaturalN a{0U};
    a += NaturalN{1U};
    REQUIRE(a == NaturalN{1U});
  }

  SECTION("1+1") {
    NaturalN a{1U};
    a += NaturalN{1U};
    REQUIRE(a == NaturalN{2U});
  }

  SECTION("1+2") {
    NaturalN a{1U};
    a += NaturalN{2U};
    REQUIRE(a == NaturalN{3U});
  }

  SECTION("2+1") {
    NaturalN a{2U};
    a += NaturalN{1U};
    REQUIRE(a == NaturalN{3U});
  }

  SECTION("3+1") {
    NaturalN a{3U};
    a += NaturalN{1U};
    REQUIRE(a == NaturalN{4U});
  }

  SECTION("3+2") {
    NaturalN a{3U};
    a += NaturalN{2U};
    REQUIRE(a == NaturalN{5U});
  }

  SECTION("3+3") {
    NaturalN a{3U};
    a += NaturalN{3U};
    REQUIRE(a == NaturalN{6U});
  }

  SECTION("3+4") {
    NaturalN a{3U};
    NaturalN b{4U};
    a += b;
    REQUIRE(a == NaturalN{7U});
  }

  SECTION("3+5") {
    NaturalN a{3U};
    NaturalN b{5U};
    a += b;
    REQUIRE(a == NaturalN{8U});
  }

  SECTION("3+6") {
    NaturalN a{3U};
    NaturalN b{6U};
    a += b;
    REQUIRE(a == NaturalN{9U});
  }

  SECTION("4+1") {
    NaturalN a{4U};
    a += NaturalN{1U};
    REQUIRE(a == NaturalN{5U});
  }

  SECTION("1+10") {
    NaturalN a{1U};
    a += NaturalN{10U};
    REQUIRE(a == NaturalN{11U});
  }

  SECTION("Arbitrary Number + Arbitrary Number") {
    NaturalN a{41983U};
    NaturalN b{2958912349U};

    a += b;
    REQUIRE(a == NaturalN{41983U + 2958912349U});
  }
}

TEST_CASE("NaturalN Subtraction", "") {
  SECTION("0-0") {
    NaturalN a{0U};
    a -= NaturalN{0U};
    REQUIRE(a == NaturalN{0U});
  }

  SECTION("a - a == 0") {
    NaturalN a{12381923U};
    NaturalN b{a};

    a -= b;
    REQUIRE(a == 0_U);
  }

  SECTION("6 - 5 == 1") {
    NaturalN a{6U};
    NaturalN b{5U};
    a -= b;
    REQUIRE(a == 1_U);
  }

  SECTION("6 - 1 == 5") {
    NaturalN a{6U};
    NaturalN b{1U};
    a -= b;
    REQUIRE(a == 5_U);
  }

  SECTION("2 - 1 == 1") {
    NaturalN a{2U};
    NaturalN b{1U};
    a -= b;
    REQUIRE(a == 1_U);
  }

  SECTION("10 - 3 == 7") {
    NaturalN a{10U};
    NaturalN b{3U};

    a -= b;
    REQUIRE(a == 7_U);
  }

  SECTION("8 - 1 == 7") {
    NaturalN a{8U};
    NaturalN b{1U};
    a -= b;
    REQUIRE(a == 7_U);
  }

  SECTION("32 - 16 == 16") {
    NaturalN a{32U};
    NaturalN b{16U};
    a -= b;
    REQUIRE(a == 16_U);
  }

  SECTION("31 - 16 == 15") {
    NaturalN a{31U};
    NaturalN b{16U};
    a -= b;
    REQUIRE(a == 15_U);
  }

  SECTION("1039812U - 1239U is the same builtin result") {
    NaturalN a{1039812U};
    NaturalN b{1239U};
    a -= b;
    REQUIRE(a == 1039812_U - 1239_U);
  }

  SECTION("0 - 1 must throw a domain_error") {
    NaturalN a{0U};
    NaturalN b{1U};
    REQUIRE_THROWS_AS(a -= b, std::domain_error);
  }
  SECTION("12310914U - 112839149123U must throw a domain_error") {
    NaturalN a{12310914U};
    NaturalN b{112839149123U};
    REQUIRE_THROWS_AS(a -= b, std::domain_error);
  }
}
TEST_CASE("Multiplication", "") {
  SECTION("0 x 0") {
    NaturalN a{0U};
    a *= NaturalN{0U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("0 x 10") {
    NaturalN a{0U};
    a *= NaturalN{10U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("1 x 10") {
    NaturalN a{1U};
    NaturalN b{10U};
    a *= b;
    REQUIRE(a == NaturalN{10U});
  }
  SECTION("10 x 1") {
    NaturalN a{10U};
    a *= NaturalN{1U};
    REQUIRE(a == NaturalN{10U});
  }
  SECTION("10 x 42") {
    NaturalN a{10U};
    a *= NaturalN{42U};
    REQUIRE(a == NaturalN{420U});
  }
  SECTION("11 x 42") {
    NaturalN a{11U};
    a *= NaturalN{42U};
    REQUIRE(a == NaturalN{462U});
  }
  SECTION("11 x 53") {
    NaturalN a{11U};
    a *= NaturalN{53U};
    REQUIRE(a == NaturalN{11U * 53U});
  }

  SECTION("1283912381092 x 564123") {
    NaturalN a{1283912381092ULL};
    a *= NaturalN{564123ULL};
    REQUIRE(a == NaturalN{1283912381092ULL * 564123ULL});
  }
}

TEST_CASE("Division", "") {
  SECTION("0 / 0") {
    NaturalN a{0U};
    REQUIRE_THROWS_AS(a /= NaturalN{0U}, std::invalid_argument);
  }
  SECTION("1 / 0") {
    NaturalN a{1U};
    REQUIRE_THROWS_AS(a /= NaturalN{0U}, std::invalid_argument);
  }
  SECTION("0 / 10") {
    NaturalN a{0U};
    a /= NaturalN{10U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("1 / 10") {
    NaturalN a{1U};
    a /= NaturalN{10U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("10 / 1") {
    NaturalN a{10U};
    a /= NaturalN{1U};
    REQUIRE(a == NaturalN{10U});
  }
  SECTION("42 / 10") {
    NaturalN a{42U};
    a /= NaturalN{10U};
    REQUIRE(a == NaturalN{4U});
  }
  SECTION("1872 / 15") {
    NaturalN a{1872U};
    a /= 15_U;
    const auto [q, r] = divmod(a, NaturalN{15U});
    REQUIRE(a == 15_U * q + r);
    REQUIRE(a == 124_U);
  }

  SECTION("1283912381092 / 564123") {
    NaturalN a{1283912381092ULL};
    a /= NaturalN{564123ULL};
    REQUIRE(a == NaturalN{1283912381092ULL / 564123ULL});
  }
}

TEST_CASE("Modulus", "") {
  SECTION("0 % 0") {
    NaturalN a{0U};
    REQUIRE_THROWS_AS(a %= NaturalN{0U}, std::invalid_argument);
  }
  SECTION("1 % 0") {
    NaturalN a{1U};
    REQUIRE_THROWS_AS(a %= NaturalN{0U}, std::invalid_argument);
  }
  SECTION("0 % 10") {
    NaturalN a{0U};
    a %= NaturalN{10U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("1 % 10") {
    NaturalN a{1U};
    a %= NaturalN{10U};
    REQUIRE(a == NaturalN{1U});
  }
  SECTION("10 % 1") {
    NaturalN a{10U};
    a %= NaturalN{1U};
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("42 % 10") {
    NaturalN a{42U};
    a %= NaturalN{10U};
    REQUIRE(a == NaturalN{2U});
  }

  SECTION("1283912381092 % 564123") {
    NaturalN a{1283912381092ULL};
    a %= NaturalN{564123ULL};
    REQUIRE(a == NaturalN{1283912381092ULL % 564123ULL});
  }
}

TEST_CASE("NaturalN LeftShift", "") {
  SECTION("Shift 0 by 1 remains 0") {
    NaturalN a{0U};
    a <<= 1;
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("Shift 1 by 1 becomes 2") {
    NaturalN a{1U};
    a <<= 1;
    REQUIRE(a == NaturalN{2U});
  }
  SECTION("Shift 1 by 20 becomes 2^20") {
    NaturalN a{1U};
    a <<= 20;
    REQUIRE(a == NaturalN{1U << 20U});
  }
  SECTION("Shift weird number a couple of times equals doubing") {
    NaturalN a{11235123U};
    a <<= 4;
    REQUIRE(a == NaturalN{11235123U * 2U * 2U * 2U * 2U});
  }
}

TEST_CASE("NaturalN RightShift", "") {
  SECTION("Shift 0 by 1 remains 0") {
    NaturalN a{0U};
    a >>= 1;
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("Shift 1 by 1 becomes 0") {
    NaturalN a{1U};
    a >>= 1;
    REQUIRE(a == NaturalN{0U});
  }
  SECTION("Shift 2^20 by 20 becomes 1") {
    NaturalN a{1U << 20U};
    a >>= 20;
    REQUIRE(a == NaturalN{1U});
  }
  SECTION("Shift weird number a couple of times equals halfing") {
    NaturalN a{11235123U};
    a >>= 4;
    REQUIRE(a == NaturalN{11235123U / 2U / 2U / 2U / 2U});
  }
}

TEST_CASE("OddityCheck", "") {
  SECTION("0 is even") {
    NaturalN a{0U};
    REQUIRE(a.isEven());
    REQUIRE(!a.isOdd());
  }
  SECTION("2 is even") {
    NaturalN a{2U};
    REQUIRE(a.isEven());
    REQUIRE(!a.isOdd());
  }
  SECTION("1602134 is even") {
    NaturalN a{1602134U};
    REQUIRE(a.isEven());
    REQUIRE(!a.isOdd());
  }
  SECTION("1 is uneven") {
    NaturalN a{1U};
    REQUIRE(!a.isEven());
    REQUIRE(a.isOdd());
  }
  SECTION("15 is uneven") {
    NaturalN a{15U};
    REQUIRE(!a.isEven());
    REQUIRE(a.isOdd());
  }
  SECTION("1231241239123141 is uneven") {
    NaturalN a{1231241239123141U};
    REQUIRE(!a.isEven());
    REQUIRE(a.isOdd());
  }
}

TEST_CASE("DivMod", "") {
  SECTION("division by 0 throws exception") {
    NaturalN a{10U};
    NaturalN b{0U};
    REQUIRE_THROWS_AS(divmod(a, b), std::invalid_argument);
  }
  SECTION("0 divided by anything is 0 with no remainder") {
    NaturalN a{10U};
    NaturalN b{0U};
    REQUIRE(divmod(b, a) == std::pair{NaturalN{0U}, NaturalN{0U}});
  }
  SECTION("<small> divided by <big> is 0, remainder <small>") {
    NaturalN a{10U};
    NaturalN b{30U};
    REQUIRE(divmod(a, b) == std::pair{NaturalN{0U}, a});
  }
  SECTION("10 / 5 == {2,0}") {
    NaturalN a{10U};
    NaturalN b{5U};
    REQUIRE(divmod(a, b) == std::pair{NaturalN{2U}, NaturalN{0U}});
  }
  SECTION("13 / 5 == {2,3}") {
    NaturalN a{13U};
    NaturalN b{5U};
    REQUIRE(divmod(a, b) == std::pair{NaturalN{2U}, NaturalN{3U}});
  }
  SECTION("1239410 / 1247 == {993,1139}") {
    NaturalN a{1239410U};
    NaturalN b{1247U};
    REQUIRE(divmod(a, b) == std::pair{NaturalN{993U}, NaturalN{1139U}});
  }
}
TEST_CASE("Printing", "") {
  const auto printTwice = [](usize builtin, auto... mods) {
    std::stringstream ssBuiltin;
    (ssBuiltin << ... << mods) << builtin;

    const auto N = NaturalN{builtin};
    std::stringstream ssNaturalN;
    (ssNaturalN << ... << mods) << N;

    return std::pair{ssBuiltin.str(), ssNaturalN.str()};
  };

  const auto N = 120397124981723U;

  SECTION("Printing Base 8") {
    SECTION("Without Base") {
      const auto [builtin, own] = printTwice(N, std::oct, std::noshowbase);
      REQUIRE(builtin == "3330004367351733");
      REQUIRE(builtin == own);
    }

    SECTION("With Base") {
      const auto [builtin, own] = printTwice(N, std::oct, std::showbase);
      REQUIRE(builtin == "03330004367351733");
      REQUIRE(builtin == own);
    }
  }

  SECTION("Printing Base 10") {
    SECTION("Without Base") {
      const auto [builtin, own] = printTwice(N, std::dec, std::noshowbase);
      REQUIRE(builtin == "120397124981723");
      REQUIRE(builtin == own);
    }

    SECTION("With Base") {
      const auto [builtin, own] = printTwice(N, std::dec, std::showbase);
      REQUIRE(builtin == "120397124981723");
      REQUIRE(builtin == own);
    }
  }

  SECTION("Printing Base 16") {
    SECTION("Without Base") {
      const auto [builtin, own] = printTwice(N, std::hex, std::noshowbase);
      REQUIRE(builtin == "6d8023ddd3db");
      REQUIRE(builtin == own);
    }

    SECTION("With Base") {
      const auto [builtin, own] = printTwice(N, std::hex, std::showbase);
      REQUIRE(builtin == "0x6d8023ddd3db");
      REQUIRE(builtin == own);
    }
  }
}

TEST_CASE("Parsing", "") {
  const auto parseTwice = [](const std::string &rawValue, auto mod) {
    auto builtin = 0ULL;
    std::stringstream ssBuiltin{rawValue};
    ssBuiltin >> mod >> builtin;

    auto N = 0_U;
    std::stringstream ssNaturalN{rawValue};
    ssNaturalN >> mod >> N;

    return std::pair{builtin, N};
  };

  SECTION("Parsing Base 8") {
    const auto [builtin, own] = parseTwice("3330004367351733", std::oct);
    REQUIRE(builtin == 120397124981723ULL);
    REQUIRE(own == 120397124981723_U);
  }

  SECTION("Parsing Base 10") {
    const auto [builtin, own] = parseTwice("120397124981723", std::dec);
    REQUIRE(builtin == 120397124981723ULL);
    REQUIRE(own == 120397124981723_U);
  }
  SECTION("Parsing Base 16 small letters") {
    const auto [builtin, own] = parseTwice("6d8023ddd3db", std::hex);
    REQUIRE(builtin == 120397124981723ULL);
    REQUIRE(own == 120397124981723_U);
  }
  SECTION("Parsing Base 16 big letters") {
    const auto [builtin, own] = parseTwice("6D8023DDD3DB", std::hex);
    REQUIRE(builtin == 120397124981723ULL);
    REQUIRE(own == 120397124981723_U);
  }
}

TEST_CASE("Literal", "") {
  const auto N = 123098124_U;
  REQUIRE(N == NaturalN{123098124U});
}
