#include "jt-computing/math/BigUInt.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

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
