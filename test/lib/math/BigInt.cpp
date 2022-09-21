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
