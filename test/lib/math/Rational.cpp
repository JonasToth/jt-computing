module;

#include <catch2/catch_test_macros.hpp>

module jt.Math:TestRational;

import jt.Math;

using namespace jt;
using namespace jt::math;

TEST_CASE("Rational Construction", "") {
  SECTION("Default Construction") {
    Rational r;
    REQUIRE(r.getNumerator() == 0_Z);
    REQUIRE(r.getDenominator() == 1_N);

    r = 42_Q;
    REQUIRE(r.getNumerator() == 42_Z);
    REQUIRE(r.getDenominator() == 1_N);
  }
  SECTION("UDL") {
    const auto r = 4201_Q;
    REQUIRE(r.getNumerator() == 4201_Z);
    REQUIRE(r.getDenominator() == 1_N);
  }
  SECTION("Ratio") {
    const auto r = Rational{-19_Z, 17_Z};
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

#if 0
TEST_CASE("Rational from Double", "") {
  REQUIRE(234234.23_Q == Rational{234234});
}
#endif

TEST_CASE("Rational Comparison", "") {
  SECTION("Identical by Construction") {
    auto const lhs = Rational{14, 5_Z};
    auto const rhs = Rational{14_N, 5_Z};
    REQUIRE(lhs == rhs);
  }
  SECTION("Identical after Reduction") {
    REQUIRE(Rational{14, 7_Z} == Rational{10_N, 5_Z});
  }
  SECTION("Identical with Signs Swapped") {
    REQUIRE(Rational{-14, 7_Z} == Rational{10_N, -5_N});
  }
  SECTION("Difference by Sign") {
    REQUIRE(Rational{-14, -7_N} != Rational{10_N, -5_N});
  }
  SECTION("Difference by Numerator") {
    REQUIRE(Rational{14, 5_Z} != Rational{10_N, 5_Z});
  }
  SECTION("Difference by Denominator") {
    REQUIRE(Rational{10, 5_Z} != Rational{10_N, 7_Z});
  }
}

TEST_CASE("Rational <=>", "") {
  REQUIRE(Rational{-2, 5_Z} < Rational{3_N, 5_Z});
  REQUIRE(Rational{+2, 5_Z} < Rational{3_N, 5_Z});
  REQUIRE(Rational{-3, 5_Z} < Rational{2_N, 5_Z});
  REQUIRE(Rational{+4, 7_Z} < Rational{9_N, 15_Z});
  REQUIRE(Rational{+3, 5_Z} <= Rational{3_N, 5_Z});
  REQUIRE(Rational{+3, 5_Z} <= Rational{6_N, 10_Z});

  REQUIRE(Rational{+3, 5_Z} > Rational{2_N, 7_Z});
  REQUIRE(Rational{+2, 5_Z} > Rational{-3_N, 5_Z});
  REQUIRE(Rational{+3, 5_Z} >= Rational{3_N, 5_Z});
  REQUIRE(Rational{+3, 5_Z} >= Rational{6_N, 10_Z});
  REQUIRE(Rational{-3, 5_Z} >= Rational{-6_N, 10_Z});
}

TEST_CASE("Rational Addition", "") {
  SECTION("Add With same Denom") {
    auto r1       = Rational{11, 5_Z};
    const auto r2 = Rational{13, 5_Z};
    r1 += r2;
    REQUIRE(r1 == Rational{24, 5_Z});
  }
  SECTION("Add With same Numerator") {
    auto r1       = Rational{13, 21_Z};
    const auto r2 = Rational{13, 5_Z};
    r1 += r2;
    REQUIRE(r1 == Rational{338, 105_Z});
  }
  SECTION("Add Negatives to Cancel Out") {
    auto r1       = Rational{13, 21_Z};
    const auto r2 = Rational{-13, 21_Z};
    r1 += r2;
    REQUIRE(r1 == Rational{0, 1_Z});
  }
  SECTION("Addition Change sign") {
    auto r1       = Rational{13, 21_Z};
    const auto r2 = Rational{-42, 21_Z};
    r1 += r2;
    REQUIRE(r1 == Rational{-29, 21_Z});
  }
  SECTION("Addition for negative Rationals") {
    auto r1       = Rational{13, -21_Z};
    const auto r2 = Rational{-42, 21_Z};
    r1 += r2;
    REQUIRE(r1 == Rational{-55, 21_Z});
  }
}

TEST_CASE("Rational Subtraction", "") {
  SECTION("Comlex Subtraction") {
    auto r1       = Rational{11, 5_Z};
    const auto r2 = Rational{25, -1412_N};
    r1 -= r2;
    REQUIRE(r1 == Rational{15657, 7060_Z});
  }
}

TEST_CASE("Rational Multiplication", "") {
  SECTION("Multiplication with Whole Number") {
    auto r1 = Rational{11, 5_Z};
    r1 *= Rational{-4, 1_Z};
    REQUIRE(r1 == Rational{-44, 5_Z});
  }
  SECTION("Multiplication with Rational") {
    auto r1 = Rational{15, 5_Z};
    r1 *= Rational{-4, 24_Z};
    REQUIRE(r1 == Rational{-60, 120_Z});
  }
}

TEST_CASE("Rational Division", "") {
  SECTION("Division with Whole Number") {
    auto r1 = Rational{12, 5_Z};
    r1 /= Rational{-4, 1_Z};
    REQUIRE(r1 == Rational{-3, 5_Z});
  }
  SECTION("Division with Rational") {
    auto r1 = Rational{15, 5_Z};
    r1 /= Rational{-4, 24_Z};
    REQUIRE(r1 == Rational{-18, 1_Z});
  }
}
