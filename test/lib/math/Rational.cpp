#include "jt-computing/math/Rational.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::math;

TEST_CASE("Rational Construction", "") {
  SECTION("UDL") {
    const auto r = 4201_Q;
    REQUIRE(r.getNumerator() == 4201_Z);
    REQUIRE(r.getDenominator() == 1_N);
  }
  SECTION("Ratio") {
    const auto r = Rational{-19_Z, 17_N};
    REQUIRE(r.getNumerator() == -19_Z);
    REQUIRE(r.getDenominator() == 17_N);
  }
  SECTION("Ratio PI") {
    const auto r = Rational{22, -7_Z};
    REQUIRE(r.getNumerator() == -22_Z);
    REQUIRE(r.getDenominator() == 7_N);
  }
  SECTION("Ratio Can Be Reduced") {
    const auto r = Rational{21, -7_Z};
    REQUIRE(r.getNumerator() == -3_Z);
    REQUIRE(r.getDenominator() == 1_N);
  }
  SECTION("Ratio Can Be Reduced Builtins") {
    const auto r = Rational{412, 129847412_Z};
    REQUIRE(r.getNumerator() == 103_Z);
    REQUIRE(r.getDenominator() == 32461853_N);
  }
}

TEST_CASE("Rational from Double", "") {
  REQUIRE(234234.23_Q == Rational{234234});
}

TEST_CASE("Rational Comparison", "") {
  SECTION("Identical by Construction") {
    REQUIRE(Rational{14, 5_N} == Rational{14_N, 5_N});
  }
  SECTION("Identical after Reduction") {
    REQUIRE(Rational{14, 7_N} == Rational{10_N, 5_N});
  }
  SECTION("Identical with Signs Swapped") {
    REQUIRE(Rational{-14, 7_N} == Rational{10_N, -5_N});
  }
  SECTION("Difference by Sign") {
    REQUIRE(Rational{-14, -7_N} != Rational{10_N, -5_N});
  }
  SECTION("Difference by Numerator") {
    REQUIRE(Rational{14, 5_N} != Rational{10_N, 5_N});
  }
  SECTION("Difference by Denominator") {
    REQUIRE(Rational{10, 5_N} != Rational{10_N, 7_N});
  }
}

TEST_CASE("Rational <=>", "") {
  REQUIRE(Rational{-2, 5_N} < Rational{3_N, 5_N});
  REQUIRE(Rational{+2, 5_N} < Rational{3_N, 5_N});
  REQUIRE(Rational{-3, 5_N} < Rational{2_N, 5_N});
  REQUIRE(Rational{+4, 7_N} < Rational{9_N, 15_N});
  REQUIRE(Rational{+3, 5_N} <= Rational{3_N, 5_N});
  REQUIRE(Rational{+3, 5_N} <= Rational{6_N, 10_N});

  REQUIRE(Rational{+3, 5_N} > Rational{2_N, 7_N});
  REQUIRE(Rational{+2, 5_N} > Rational{-3_N, 5_N});
  REQUIRE(Rational{+3, 5_N} >= Rational{3_N, 5_N});
  REQUIRE(Rational{+3, 5_N} >= Rational{6_N, 10_N});
  REQUIRE(Rational{-3, 5_N} >= Rational{-6_N, 10_N});
}

TEST_CASE("Rational Addition", "") {
  SECTION("Add With same Denom") {
    auto r1       = Rational{11, 5_N};
    const auto r2 = Rational{13, 5_N};
    r1 += r2;
    REQUIRE(r1 == Rational{24, 5_N});
  }
  SECTION("Add With same Numerator") {
    auto r1       = Rational{13, 21_N};
    const auto r2 = Rational{13, 5_N};
    r1 += r2;
    REQUIRE(r1 == Rational{338, 105_N});
  }
  SECTION("Add Negatives to Cancel Out") {
    auto r1       = Rational{13, 21_N};
    const auto r2 = Rational{-13, 21_N};
    r1 += r2;
    REQUIRE(r1 == Rational{0});
  }
  SECTION("Addition Change sign") {
    auto r1       = Rational{13, 21_N};
    const auto r2 = Rational{-42, 21_N};
    r1 += r2;
    REQUIRE(r1 == Rational{-29, 21_N});
  }
  SECTION("Addition for negative Rationals") {
    auto r1       = Rational{13, -21_N};
    const auto r2 = Rational{-42, 21_N};
    r1 += r2;
    REQUIRE(r1 == Rational{-55, 21_N});
  }
}

TEST_CASE("Rational Subtraction", "") {
  SECTION("Comlex Subtraction") {
    auto r1       = Rational{11, 5_N};
    const auto r2 = Rational{25, -1412_N};
    r1 -= r2;
    REQUIRE(r1 == Rational{15657, 7060_N});
  }
}

TEST_CASE("Rational Multiplication", "") {
  SECTION("Multiplication with Whole Number") {
    auto r1 = Rational{11, 5_N};
    r1 *= Rational{-4, 1_N};
    REQUIRE(r1 == Rational{-44, 5_N});
  }
  SECTION("Multiplication with Rational") {
    auto r1 = Rational{15, 5_N};
    r1 *= Rational{-4, 24_N};
    REQUIRE(r1 == Rational{-60, 120_N});
  }
}

TEST_CASE("Rational Division", "") {
  SECTION("Division with Whole Number") {
    auto r1 = Rational{12, 5_N};
    r1 /= Rational{-4, 1_N};
    REQUIRE(r1 == Rational{-3, 5_N});
  }
  SECTION("Division with Rational") {
    auto r1 = Rational{15, 5_N};
    r1 /= Rational{-4, 24_N};
    REQUIRE(r1 == Rational{-18, 1_N});
  }
}
