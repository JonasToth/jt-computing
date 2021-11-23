#include "jt-computing/container/BitVector.hpp"
#include "jt-computing/Types.hpp"

#include <limits>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::container;

TEST_CASE("BitVector Construction", "") {
  SECTION("Default Construction") {
    BitVector b;
    REQUIRE(b.capacity() == 0);
    REQUIRE(b.size() == 0);
  }

  SECTION("With Small Size") {
    BitVector b{7ULL, false};
    REQUIRE(b.capacity() >= 7);
    REQUIRE(b.size() == 7ULL);
  }

  SECTION("With Bigger Sizes") {
    BitVector b{214ULL, false};
    REQUIRE(b.capacity() >= 214ULL);
    REQUIRE(b.size() == 214ULL);
  }

  SECTION("With unsigned 8bit integer") {
    SECTION("All 0") {
      BitVector b{u8{0}};
      REQUIRE(b.capacity() == 8ULL);
      REQUIRE(b.size() == 8ULL);
      for (usize i = 0; i < 8; ++i) {
        REQUIRE(b.get(i) == false);
      }
    }

    SECTION("All 1") {
      BitVector b{std::numeric_limits<u8>::max()};
      REQUIRE(b.capacity() == 8ULL);
      REQUIRE(b.size() == 8ULL);
      for (usize i = 0; i < 8; ++i) {
        REQUIRE(b.get(i) == true);
      }
    }

    SECTION("First half 0, second 1") {
      BitVector b{u8{240}};
      REQUIRE(b.capacity() == 8ULL);
      REQUIRE(b.size() == 8ULL);
      usize i = 0;
      for (i = 0; i < 4; ++i) {
        REQUIRE(b.get(i) == false);
      }
      for (i = 4; i < 8; ++i) {
        REQUIRE(b.get(i) == true);
      }
    }

    SECTION("First half 1, second 0") {
      BitVector b{u8{15}};
      REQUIRE(b.capacity() == 8ULL);
      REQUIRE(b.size() == 8ULL);
      usize i = 0;
      for (i = 0; i < 4; ++i) {
        REQUIRE(b.get(i) == true);
      }
      for (i = 4; i < 8; ++i) {
        REQUIRE(b.get(i) == false);
      }
    }
  }
}

TEST_CASE("BitVector Appending", "") {
  // Bit 4 must be set to 1, the rest is 0
  BitVector b{u8{16}};
  REQUIRE(b.size() == 8);
  REQUIRE(b.get(0) == false);
  REQUIRE(b.get(1) == false);
  REQUIRE(b.get(2) == false);
  REQUIRE(b.get(3) == false);
  REQUIRE(b.get(4) == true);
  REQUIRE(b.get(5) == false);
  REQUIRE(b.get(6) == false);
  REQUIRE(b.get(7) == false);
  b.push_back(false);
  b.push_back(true);
  b.push_back(true);
  b.push_back(false);

  REQUIRE(b.size() == 12);
  REQUIRE(b.get(8) == false);
  REQUIRE(b.get(9) == true);
  REQUIRE(b.get(10) == true);
  REQUIRE(b.get(11) == false);
}

TEST_CASE("BitVector normalization", "") {
  SECTION("From u8 type") {
    BitVector b{u8{16}};
    b.normalize();
    REQUIRE(b.size() == 5);
  }

  SECTION("From u32 type") {
    BitVector b{u32{16}};
    b.normalize();
    REQUIRE(b.size() == 5);
  }
}

TEST_CASE("BitVector left shifting", "") {
  BitVector b{u8{16}};
  REQUIRE(b.size() == 8);

  b <<= 4;
  REQUIRE(b.size() == 12);
  REQUIRE(b.get(0) == false);
  REQUIRE(b.get(1) == false);
  REQUIRE(b.get(2) == false);
  REQUIRE(b.get(3) == false);
  REQUIRE(b.get(8) == true);
  REQUIRE(b.get(11) == false);

  b.normalize();
  REQUIRE(b.size() == 9);
}

TEST_CASE("BitVector right shifting", "") {
  BitVector b{u8{31}};
  REQUIRE(b.size() == 8);

  b >>= 2;
  REQUIRE(b.size() == 6);
  REQUIRE(b.get(0) == true);
  REQUIRE(b.get(1) == true);
  REQUIRE(b.get(2) == true);
  REQUIRE(b.get(3) == false);
  REQUIRE(b.get(4) == false);
  REQUIRE(b.get(5) == false);

  b.normalize();
  REQUIRE(b.size() == 3);
}
