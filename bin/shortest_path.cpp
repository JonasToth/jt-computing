#include "jt-computing/core/Contracts.hpp"

import std;
import jt.Core;
import jt.Math;

using namespace std;
using namespace jt;
using namespace jt::math;

namespace {
/// Represents positive distances.
class Dist {
public:
  constexpr Dist() = default;
  explicit constexpr Dist(i32 d) : value{d} { /*assert(d >= 0);*/ }

  static constexpr Dist inf() noexcept {
    return Dist{numeric_limits<i32>::max()};
  }
  [[nodiscard]] constexpr bool isInf() const noexcept {
    return value == numeric_limits<i32>::max();
  }

  constexpr Dist &operator+=(const Dist &other) {
    if (isInf() || other.isInf()) {
      value = numeric_limits<i32>::max();
    } else {
      auto r = i64{value} + i64{other.value};
      value  = r < numeric_limits<i32>::max() ? static_cast<i32>(r)
                                              : numeric_limits<i32>::max();
    }
    return *this;
  }

  [[nodiscard]] constexpr strong_ordering operator<=>(const Dist &other) const {
    return value <=> other.value;
  }
  [[nodiscard]] constexpr bool operator==(const Dist &other) const {
    return value == other.value;
  }
  friend ostream &operator<<(ostream &os, const Dist &d);

private:
  i32 value{0};
};

inline constexpr Dist operator+(const Dist &d1, const Dist &d2) {
  if (d1.isInf() || d2.isInf()) {
    return Dist::inf();
  }
  auto result{d1};
  return result += d2;
}
inline ostream &operator<<(ostream &os, const Dist &d) {
  if (d.isInf()) {
    os << "inf";
  } else {
    os << setw(3) << d.value;
  }
  return os;
}

struct MinimalDist {
  constexpr Dist operator()(const Dist &d1, const Dist &d2) const {
    return min(d1, d2);
  }
};
inline constexpr Dist identity_element(MinimalDist /*d*/) {
  return Dist::inf();
}
} // namespace

int main(int /*argc*/, char ** /*argv*/) {
  // clang-format off
  // NOLINTBEGIN
  constexpr auto N = 7;
  const auto distances = FixedSquareMatrix<Dist, N, MinimalDist, plus<Dist>>{
      Dist{0}    , Dist{6}    , Dist::inf(), Dist{3}    , Dist::inf(), Dist::inf(), Dist::inf(),
      Dist::inf(), Dist{0}    , Dist::inf(), Dist::inf(), Dist{2}    , Dist{10}   , Dist::inf(),
      Dist{7}    , Dist::inf(), Dist{0}    , Dist::inf(), Dist::inf(), Dist::inf(), Dist::inf(),
      Dist::inf(), Dist::inf(), Dist{5}    , Dist{0}    , Dist::inf(), Dist{4}    , Dist::inf(),
      Dist::inf(), Dist::inf(), Dist::inf(), Dist::inf(), Dist{0}    , Dist::inf(), Dist{3}    ,
      Dist::inf(), Dist::inf(), Dist{6}    , Dist::inf(), Dist{7}    , Dist{0}    , Dist{8}    ,
      Dist::inf(), Dist{9}    , Dist::inf(), Dist::inf(), Dist::inf(), Dist::inf(), Dist{0}    ,
  };
  // NOLINTEND
  // clang-format on

  cout << "Initial Directed Distances:" << endl;
  cout << distances << endl;

  const auto shortestPathLengths = power_monoid(distances, N - 1);
  cout << "Shortest Path Lengths between each Node:" << endl;
  cout << shortestPathLengths << endl;

  return jt::EXIT_SUCCESS;
}
