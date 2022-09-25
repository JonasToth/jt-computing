#include "jt-computing/math/BigInt.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

TEST_CASE("BigInt Construction", "") {
  SECTION("0") {
    BigInt bi0{0};
    REQUIRE(!bi0.isNegative());
    REQUIRE(bi0.abs() == BigUInt{0U});

    BigInt bi1{-0};
    REQUIRE(!bi1.isNegative());
    REQUIRE(bi1.abs() == BigUInt{0U});
  }
  SECTION("-10") {
    BigInt bi{-10};
    REQUIRE(bi.isNegative());
    REQUIRE(bi.abs() == BigUInt{10U});
  }
  SECTION("10") {
    BigInt bi{10};
    REQUIRE(!bi.isNegative());
    REQUIRE(bi.abs() == BigUInt{10U});
  }
  SECTION("other builtin types") {
    BigInt bi0{10LL};
    REQUIRE(!bi0.isNegative());
    REQUIRE(bi0.abs() == BigUInt{10U});

    BigInt bi1{-10LL};
    REQUIRE(bi1.isNegative());
    REQUIRE(bi1.abs() == BigUInt{10U});

    BigInt bi2{0UL};
    REQUIRE(!bi2.isNegative());
    REQUIRE(bi2.abs() == BigUInt{0U});

    BigInt bi3{0U};
    REQUIRE(!bi3.isNegative());
    REQUIRE(bi3.abs() == BigUInt{0U});

    BigInt bi4{10231U};
    REQUIRE(!bi4.isNegative());
    REQUIRE(bi4.abs() == BigUInt{10231U});
  }

  SECTION("Literal") {
    const auto bi1 = -10_Z;
    REQUIRE(bi1.isNegative());
    REQUIRE(bi1.abs() == 10_N);

    const auto bi2 = 129387123_Z;
    REQUIRE(!bi2.isNegative());
    REQUIRE(bi2.abs() == 129387123_N);
  }
}

TEST_CASE("BigInt Comparison", "") {
  SECTION("0 comparisons") {
    BigInt bi{0};
    REQUIRE(bi == 0);
    REQUIRE(bi == 0U);
    REQUIRE(bi == BigUInt{0U});
  }
  SECTION("equality") {
    REQUIRE(BigInt{-10} == BigInt{-10});
    REQUIRE(BigInt{-10} == -10);
    REQUIRE(BigInt{10} == 10U);
    REQUIRE(BigInt{10} == 10);
    REQUIRE(BigInt{10} == BigUInt{10U});
  }
  SECTION("inequality") {
    REQUIRE(BigInt{-10} != BigInt{-9});
    REQUIRE(BigInt{-10} != -9);
    REQUIRE(BigInt{-10} != 9U);
    REQUIRE(BigInt{-10} != BigUInt{9U});

    REQUIRE(BigInt{10} != BigInt{-9});
    REQUIRE(BigInt{10} != -9);
    REQUIRE(BigInt{10} != 9U);
    REQUIRE(BigInt{10} != BigUInt{9U});
  }

  SECTION("<-relation") {
    REQUIRE(BigInt{-10} < BigInt{-9});
    REQUIRE(BigInt{-10} < -9);
    REQUIRE(BigInt{-10} < 9U);
    REQUIRE(BigInt{-10} < BigUInt{9U});

    REQUIRE(BigInt{-11} < BigInt{-10});
    REQUIRE(-10 < BigInt{-9});
    REQUIRE(-10LL < BigInt{9U});
  }

  SECTION(">-relation") {
    REQUIRE(BigInt{-1} > BigInt{-120314});
    REQUIRE(BigInt{-9} > -10);
    REQUIRE(BigInt{10} > 9U);
    REQUIRE(BigInt{10} > BigUInt{9U});

    REQUIRE(BigInt{-11} > BigInt{-12});
    REQUIRE(-9 > BigInt{-10});
    REQUIRE(10LL > BigInt{-129});
  }

  SECTION("<=-relation") {
    REQUIRE(BigInt{-10} <= BigInt{-9});
    REQUIRE(BigInt{-10} <= BigInt{-10});
    REQUIRE(BigInt{-10} <= -9);
    REQUIRE(BigInt{-10} <= -10);
    REQUIRE(BigInt{-10} <= 9U);
    REQUIRE(BigInt{-10} <= 10U);
    REQUIRE(BigInt{-10} <= BigUInt{9U});
    REQUIRE(BigInt{-10} <= BigUInt{10U});

    REQUIRE(BigInt{-11} <= BigInt{-10});
    REQUIRE(BigInt{-11} <= BigInt{-11});
    REQUIRE(-10 <= BigInt{-9});
    REQUIRE(-10 <= BigInt{-10});
    REQUIRE(-10LL <= BigInt{10U});
    REQUIRE(10LL <= BigInt{10U});
  }

  SECTION(">=-relation") {
    REQUIRE(BigInt{-1} >= BigInt{-120314});
    REQUIRE(BigInt{-1} >= BigInt{-1});
    REQUIRE(BigInt{-9} >= -10);
    REQUIRE(BigInt{-9} >= -9);
    REQUIRE(BigInt{10} >= 9U);
    REQUIRE(BigInt{10} >= 10U);
    REQUIRE(BigInt{10} >= BigUInt{10U});
    REQUIRE(BigInt{10} >= BigUInt{10U});

    REQUIRE(BigInt{-11} >= BigInt{-12});
    REQUIRE(BigInt{-11} >= BigInt{-12});
    REQUIRE(-9 >= BigInt{-10});
    REQUIRE(-9 >= BigInt{-10LL});
    REQUIRE(10LL >= BigInt{-129});
    REQUIRE(10LL >= BigInt{10ULL});
  }
}

TEST_CASE("BigInt Computation", "") {
  SECTION("+=") {
    BigInt a{0};

    a += BigInt{0};
    REQUIRE(a == 0);

    a += BigInt{10};
    REQUIRE(a == 10);

    a += -10;
    REQUIRE(a == 0);

    a += -10;
    REQUIRE(a == -10);

    a += -10;
    REQUIRE(a == -20);

    a += 20U;
    REQUIRE(a == 0);

    a += BigUInt{20U};
    REQUIRE(a == 20U);
  }

  SECTION("-=") {
    BigInt a{0};

    a -= BigInt{0};
    REQUIRE(a == 0);

    a -= BigInt{10};
    REQUIRE(a == -10);

    a -= -10;
    REQUIRE(a == 0);

    a -= 10U;
    REQUIRE(a == -10);

    a -= BigInt{-20};
    REQUIRE(a == 10);

    a -= BigUInt{5U};
    REQUIRE(a == 5U);
  }

  SECTION("*=") {
    BigInt a{0};
    a *= BigInt{-1};
    REQUIRE(a == 0);
    REQUIRE(!a.isNegative());

    a *= BigInt{0};
    REQUIRE(a == 0);

    a += 1;
    REQUIRE(a == 1);

    a *= BigInt{10};
    REQUIRE(a == 10);

    a *= -10;
    REQUIRE(a == -100);

    a *= BigInt{-10};
    REQUIRE(a == 1000);

    a *= BigInt{-1};
    REQUIRE(a == -1000);

    a *= BigUInt{2U};
    REQUIRE(a == -2000);
  }

  SECTION("/=") {
    BigInt a{0};
    a /= -1;
    REQUIRE(!a.isNegative());
    REQUIRE(a == 0);

    REQUIRE_THROWS_AS(a /= BigInt{0}, std::invalid_argument);
    REQUIRE(a == 0);

    a += 112351;
    REQUIRE(a == BigInt{112351});

    a /= BigInt{10};
    REQUIRE(a == 11235);

    a /= -2;
    REQUIRE(a == -5617);

    a /= BigInt{-1};
    REQUIRE(a == BigUInt{5617U});

    a /= 3U;
    REQUIRE(a == 1872);

    a = -a;
    REQUIRE(a == -1872);

    a /= BigUInt{15U};
    REQUIRE(a == -124);
  }
}

TEST_CASE("Printing", "") {
  const auto printTwice = [](i64 builtin, auto... mods) {
    std::stringstream ssBuiltin;
    (ssBuiltin << ... << mods) << builtin;

    const auto N = BigInt{builtin};
    std::stringstream ssBigUInt;
    (ssBigUInt << ... << mods) << N;

    return std::pair{ssBuiltin.str(), ssBigUInt.str()};
  };

  const auto NPos = +7124981723LL;
  const auto NNeg = -7124981723LL;

  SECTION("Printing Base 8") {
    SECTION("Without Base") {
      const auto [builtinPos, ownPos] =
          printTwice(NPos, std::oct, std::noshowbase);
      REQUIRE(builtinPos == "65053513733");
      REQUIRE(builtinPos == ownPos);

      const auto [builtinNeg, ownNeg] =
          printTwice(NNeg, std::oct, std::noshowbase);
      REQUIRE(builtinNeg == "1777777777712724264045");
      REQUIRE(ownNeg == "-65053513733");
    }

    SECTION("With Base") {
      const auto [builtinPos, ownPos] =
          printTwice(NPos, std::oct, std::showbase);
      REQUIRE(builtinPos == "065053513733");
      REQUIRE(builtinPos == ownPos);

      const auto [builtinNeg, ownNeg] =
          printTwice(NNeg, std::oct, std::showbase);
      REQUIRE(builtinNeg == "01777777777712724264045");
      REQUIRE(ownNeg == "-065053513733");
    }
  }

  SECTION("Printing Base 10") {
    SECTION("Without Base") {
      const auto [builtinPos, ownPos] =
          printTwice(NPos, std::dec, std::noshowbase);
      REQUIRE(builtinPos == "7124981723");
      REQUIRE(builtinPos == ownPos);

      const auto [builtinNeg, ownNeg] =
          printTwice(NNeg, std::dec, std::noshowbase);
      REQUIRE(builtinNeg == "-7124981723");
      REQUIRE(ownNeg == "-7124981723");
    }

    SECTION("With Base") {
      const auto [builtinPos, ownPos] =
          printTwice(NPos, std::dec, std::showbase);
      REQUIRE(builtinPos == "7124981723");
      REQUIRE(builtinPos == ownPos);

      const auto [builtinNeg, ownNeg] =
          printTwice(NNeg, std::dec, std::showbase);
      REQUIRE(builtinNeg == "-7124981723");
      REQUIRE(ownNeg == "-7124981723");
    }
  }

  SECTION("Printing Base 16") {
    SECTION("Without Base") {
      const auto [builtinPos, ownPos] =
          printTwice(NPos, std::hex, std::noshowbase);
      REQUIRE(builtinPos == "1a8ae97db");
      REQUIRE(builtinPos == ownPos);

      const auto [builtinNeg, ownNeg] =
          printTwice(NNeg, std::hex, std::noshowbase);
      REQUIRE(builtinNeg == "fffffffe57516825");
      REQUIRE(ownNeg == "-1a8ae97db");
    }

    SECTION("With Base") {
      const auto [builtinPos, ownPos] =
          printTwice(NPos, std::hex, std::showbase);
      REQUIRE(builtinPos == "0x1a8ae97db");
      REQUIRE(builtinPos == ownPos);

      const auto [builtinNeg, ownNeg] =
          printTwice(NNeg, std::hex, std::showbase);
      REQUIRE(builtinNeg == "0xfffffffe57516825");
      REQUIRE(ownNeg == "-0x1a8ae97db");
    }
  }
}

TEST_CASE("Parsing", "") {
  const auto parseTwice = [](const std::string &rawValue, auto mod) {
    auto builtin = 0LL;
    std::stringstream ssBuiltin{rawValue};
    ssBuiltin >> mod >> builtin;

    auto N = 0_Z;
    std::stringstream ssBigInt{rawValue};
    ssBigInt >> mod >> N;

    return std::pair{builtin, N};
  };

  SECTION("Parsing Base 8") {
    const auto [builtinPos, ownPos] = parseTwice("3330004367351733", std::oct);
    REQUIRE(builtinPos == 120397124981723LL);
    REQUIRE(ownPos == 120397124981723_Z);

    const auto [builtinNeg, ownNeg] = parseTwice("-3330004367351733", std::oct);
    REQUIRE(builtinNeg == -120397124981723LL);
    REQUIRE(ownNeg == -120397124981723_Z);
  }

  SECTION("Parsing Base 10") {
    const auto [builtinPos, ownPos] = parseTwice("120397124981723", std::dec);
    REQUIRE(builtinPos == 120397124981723LL);
    REQUIRE(ownPos == 120397124981723_Z);

    const auto [builtinNeg, ownNeg] = parseTwice("-120397124981723", std::dec);
    REQUIRE(builtinNeg == -120397124981723LL);
    REQUIRE(ownNeg == -120397124981723_Z);
  }
  SECTION("Parsing Base 16 small letters") {
    const auto [builtinPos, ownPos] = parseTwice("6d8023ddd3db", std::hex);
    REQUIRE(builtinPos == 120397124981723LL);
    REQUIRE(ownPos == 120397124981723_Z);

    const auto [builtinNeg, ownNeg] = parseTwice("-6d8023ddd3db", std::hex);
    REQUIRE(builtinNeg == -120397124981723LL);
    REQUIRE(ownNeg == -120397124981723_Z);
  }
  SECTION("Parsing Base 16 big letters") {
    const auto [builtinPos, ownPos] = parseTwice("6D8023DDD3DB", std::hex);
    REQUIRE(builtinPos == 120397124981723LL);
    REQUIRE(ownPos == 120397124981723_Z);

    const auto [builtinNeg, ownNeg] = parseTwice("-6D8023DDD3DB", std::hex);
    REQUIRE(builtinNeg == -120397124981723LL);
    REQUIRE(ownNeg == -120397124981723_Z);
  }
}
