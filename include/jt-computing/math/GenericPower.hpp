#pragma once

#include "jt-computing/math/AlgebraConcepts.hpp"

#include <cassert>
#include <functional>

namespace jt::math {

bool isOdd(const Integer auto &n) { return n & 1; }
bool isEven(const Integer auto &n) { return !isOdd(n); }

namespace detail {
template <NoncommutativeAdditiveSemigroup A, Integer N>
A multiply_accumulate_semigroup(A r, N n, A a) {
  assert(n > 0);
  if (n == N{0U}) {
    return r;
  }

  while (true) {
    if (isOdd(n)) {
      r += a;

      if (n == N{1U}) {
        return r;
      }
    }
    n /= N{2U};
    a += a;
  }
}

template <NoncommutativeAdditiveSemigroup A, Integer N>
A multiply_semigroup(N n, A a) {
  assert(n > N{0U});
  while (!isOdd(n)) {
    a += a;
    n /= N{2U};
  }
  if (n == N{1U}) {
    return a;
  }
  return multiply_accumulate_semigroup(a, (n - N{1U}) / N{2U}, a + a);
}

template <NoncommutativeAdditiveMonoid A, Integer N>
A multiply_monoid(N n, A a) {
  assert(n >= N{0U});
  if (n == N{0U}) {
    return A{0U};
  }
  return multiply_semigroup(n, a);
}

template <NoncommutativeAdditiveGroup A, Integer N> A multiply_group(N n, A a) {
  if (n < N{0U}) {
    n = -n;
    a = -a;
  }
  return multiply_monoid(n, a);
}

template <std::regular A, Integer N, Semigroup<A> Op>
A power_accumulate_semigroup(A r, A a, N n, Op op) {
  assert(n >= N{0U});
  if (n == N{0U}) {
    return r;
  }

  while (true) {
    if (isOdd(n)) {
      r = op(r, a);
      if (n == N{1U}) {
        return r;
      }
    }
    n /= N{2U};
    a = op(a, a);
  }
}
} // namespace detail

template <std::regular A, Integer N, Semigroup<A> Op = std::multiplies<A>>
A power_semigroup(A a, N n, Op op = {}) {
  assert(n > N{0U});
  while (!isOdd(n)) {
    a = op(a, a);
    n /= N{2U};
  }
  if (n == N{1U}) {
    return a;
  }
  return detail::power_accumulate_semigroup(a, op(a, a), (n - N{1U}) / N{2U},
                                            op);
}

template <std::regular A, Integer N, Monoid<A> Op = std::multiplies<A>>
A power_monoid(A a, N n, Op op = {}) {
  assert(n >= N{0U});
  if (n == N{0U}) {
    return identity_element(op);
  }
  return power_semigroup(std::move(a), std::move(n), std::move(op));
}

template <std::regular A, Integer N, Group<A> Op = std::multiplies<A>>
A power_group(A a, N n, Op op = {}) {
  if (n < N{0U}) {
    n = -n;
    a = inverse_operation(op)(a);
  }
  return power_monoid(std::move(a), std::move(n), std::move(op));
}
} // namespace jt::math
