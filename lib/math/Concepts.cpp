module;

#include "jt-computing/core/Contracts.hpp"

export module jt.Math:Concepts;

import std;
using namespace std;

// -- Algebra Concepts --
export namespace jt::math {
// clang-format off
template <typename Op, typename A>
concept Semigroup = requires(Op op, A a, A b) {
     { regular<A>};
     { op(a, b) } -> same_as<A>;
};

template <regular T>
constexpr T identity_element(plus<T> /*op*/) { return T{0U}; }

template <regular T>
constexpr T identity_element(logical_or<T> /*op*/) { return T{false}; }

template <regular T>
constexpr T identity_element(multiplies<T> /*op*/) { return T{1U}; }

template <regular T>
constexpr T identity_element(logical_and<T> /*op*/) { return T{true}; }

template <typename Op, typename A>
concept Monoid = requires(Op op, A a) {
    { Semigroup<Op, A> };
    { identity_element(op) } -> same_as<A>;
};

template <regular T>
constexpr negate<T> inverse_operation(plus<T> /*op*/) { return negate<T>{}; }

template <regular T>
struct reciprocal {
    constexpr T operator()(const T& x) const {
        CONTRACT_ASSERT(x != T{0U});
        return T{1U} / x;
    }
};
template <regular T>
constexpr reciprocal<T> inverse_operation(multiplies<T> /*op*/) { return reciprocal<T>{}; }

template <typename Op, typename A>
concept Group = requires(Op op, A a) {
    { Monoid<Op, A> };
    { inverse_operation(op) } -> same_as<A>;
};

template <typename OpPlus, typename OpTimes, typename A>
concept SemiRing = requires(OpPlus plus, OpTimes times, A a, A b) {
    { plus(a, b) } -> same_as<A>;
    { times(a, b) } -> same_as<A>;

    { identity_element(plus) } -> same_as<A>;
    { identity_element(times) } -> same_as<A>;
    { identity_element(plus) != identity_element(times) };

    { plus(a, identity_element(plus)) == a };
    { plus(identity_element(plus), a) == a };

    { times(identity_element(times), a) == a };
    { times(a, identity_element(times)) == a };

    { times(identity_element(plus), a) == identity_element(plus) };
    { times(a, identity_element(plus)) == identity_element(plus) };

    { times(a, plus(a, b)) } -> same_as<A>;
    { times(plus(a, b), b) } -> same_as<A>;
};
// clang-format on
} // namespace jt::math

// -- Number Concepts --

export namespace jt::math {

// clang-format off
template <typename N>
concept Computable = requires(N a, N b, N c) {
  { a + b } -> same_as<N>;
  { a - b } -> same_as<N>;
  { a * b } -> same_as<N>;
  { a / b } -> same_as<N>;
  { a % b } -> same_as<N>;

  { c += b } -> same_as<N &>;
  { c -= b } -> same_as<N &>;
  { c *= b } -> same_as<N &>;
  { c /= b } -> same_as<N &>;
  { c %= b } -> same_as<N &>;
};

template <typename N>
concept NaturalNumber = unsigned_integral<N> ||
                        (Computable<N> &&
                         regular<N> &&
                         totally_ordered<N> &&
                         !signed_integral<N>);

template <typename N>
concept Integer = (unsigned_integral<N> || signed_integral<N> || NaturalNumber<N>) && 
                  Computable<N> && regular<N> && totally_ordered<N>;

// clang-format on

} // namespace jt::math
