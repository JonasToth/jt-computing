#include "jt-computing/Types.hpp"
#include "jt-computing/math/FixedSquareMatrix.hpp"
#include "jt-computing/math/GenericPower.hpp"

#include <compare>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;
using namespace jt;
using namespace jt::math;

namespace {
/// Represents positive distances.
class Dist {
public:
  Dist() = default;
  explicit Dist(i32 d) : value{d} { assert(d >= 0); }

  static Dist inf() noexcept { return Dist{std::numeric_limits<i32>::max()}; }
  [[nodiscard]] bool isInf() const noexcept {
    return value == std::numeric_limits<i32>::max();
  }

  Dist &operator+=(const Dist &other) {
    if (isInf() || other.isInf()) {
      value = std::numeric_limits<i32>::max();
    } else {
      auto r = i64{value} + i64{other.value};
      value  = r < std::numeric_limits<i32>::max()
                 ? static_cast<i32>(r)
                 : std::numeric_limits<i32>::max();
    }
    return *this;
  }

  [[nodiscard]] std::strong_ordering operator<=>(const Dist &other) const {
    return value <=> other.value;
  }
  [[nodiscard]] bool operator==(const Dist &other) const {
    return value == other.value;
  }
  friend std::ostream &operator<<(std::ostream &os, const Dist &d);

private:
  i32 value{0};
};

inline Dist operator+(const Dist &d1, const Dist &d2) {
  if (d1.isInf() || d2.isInf()) {
    return Dist::inf();
  }
  auto result{d1};
  return result += d2;
}
inline std::ostream &operator<<(std::ostream &os, const Dist &d) {
  if (d.isInf()) {
    os << "inf";
  } else {
    os << std::setw(3) << d.value;
  }
  return os;
}

template <int N>
FixedSquareMatrix<Dist, N> operator*(const FixedSquareMatrix<Dist, N> &a,
                                     const FixedSquareMatrix<Dist, N> &b) {
  auto result = FixedSquareMatrix<Dist, N>(0U);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      result(i, j) = Dist::inf();
    }
  }
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        result(i, j) = std::min(result(i, j), a(i, k) + b(k, j));
      }
    }
  }
  return result;
}
} // namespace

int main(int /*argc*/, char ** /*argv*/) {
  // clang-format off
  // NOLINTBEGIN
  constexpr auto N = 7;
  const auto distances = FixedSquareMatrix<Dist, N>{
  Dist{0}     , Dist{6}     , Dist::inf() , Dist{3}     , Dist::inf() , Dist::inf() , Dist::inf() ,
  Dist::inf() , Dist{0}     , Dist::inf() , Dist::inf() , Dist{2}     , Dist{10}    , Dist::inf() ,
  Dist{7}     , Dist::inf() , Dist{0}     , Dist::inf() , Dist::inf() , Dist::inf() , Dist::inf() ,
  Dist::inf() , Dist::inf() , Dist{5}     , Dist{0}     , Dist::inf() , Dist{4}     , Dist::inf() ,
  Dist::inf() , Dist::inf() , Dist::inf() , Dist::inf() , Dist{0}     , Dist::inf() , Dist{3}     ,
  Dist::inf() , Dist::inf() , Dist{6}     , Dist::inf() , Dist{7}     , Dist{0}     , Dist{8}     ,
  Dist::inf() , Dist{9}     , Dist::inf() , Dist::inf() , Dist::inf() , Dist::inf() , Dist{0}     ,
  };
  // NOLINTEND
  // clang-format on

  std::cout << distances << std::endl;

  const auto shortestPathLengths = power_monoid(distances, N - 1);
  std::cout << shortestPathLengths << std::endl;

  return EXIT_SUCCESS;
}
