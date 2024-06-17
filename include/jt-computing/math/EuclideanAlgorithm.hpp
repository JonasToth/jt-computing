#pragma once

#include "jt-computing/math/Concepts.hpp"

#include <cassert>
#include <utility>

namespace jt::math {

template <NaturalNumber N> N euclidean_gcd(N a, N b) {
  while (true) {
    if (a < b) {
      using std::swap;
      swap(a, b);
    }
    if (b == N{0U}) {
      return a;
    }
    auto mod = a % b;
    a        = std::move(b);
    b        = std::move(mod);
  }
  std::unreachable();
}

template <NaturalNumber N> N lcm(N a, N b, N gcd) {
  if (a == N{0U} && b == N{0U}) {
    return N{0U};
  }
  if (a > b) {
    return b * (a / gcd);
  }
  return a * (b / gcd);
}
template <NaturalNumber N> N euclidean_lcm(N a, N b) {
  auto gcd = euclidean_gcd(a, b);
  return lcm(a, b, gcd);
}
} // namespace jt::math
