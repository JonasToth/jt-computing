module;

#include "jt-computing/core/Contracts.hpp"

export module jt.Math:GenericPower;

import :Concepts;

using namespace std;

export namespace jt::math {

bool isOdd(const Integer auto &n) { return n & 1; }
bool isEven(const Integer auto &n) { return !isOdd(n); }

template <regular A, Integer N, Semigroup<A> Op>
A power_accumulate_semigroup(A r, A a, N n, Op op) PRE(n >= N{0U}) {
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

template <regular A, Integer N, Semigroup<A> Op = multiplies<A>>
A power_semigroup(A a, N n, Op op = {}) PRE(n > N{0U}) {
  while (!isOdd(n)) {
    a = op(a, a);
    n /= N{2U};
  }
  if (n == N{1U}) {
    return a;
  }
  return power_accumulate_semigroup(a, op(a, a), (n - N{1U}) / N{2U}, op);
}

template <regular A, Integer N, Monoid<A> Op = multiplies<A>>
A power_monoid(A a, N n, Op op = {}) PRE(n >= N{0U}) {
  if (n == N{0U}) {
    return identity_element(op);
  }
  return power_semigroup(move(a), move(n), move(op));
}

template <regular A, Integer N, Group<A> Op = multiplies<A>>
A power_group(A a, N n, Op op = {}) {
  if (n < N{0U}) {
    n = -n;
    a = inverse_operation(op)(a);
  }
  return power_monoid(move(a), move(n), move(op));
}
} // namespace jt::math
