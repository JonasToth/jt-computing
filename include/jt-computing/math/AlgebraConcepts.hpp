#pragma once

#include "jt-computing/math/Concepts.hpp"
#include <functional>
#include <string>

namespace jt::math {
// clang-format off
template <typename Op, typename A>
concept Semigroup = requires(Op op, A a, A b) {
     { std::regular<A>};
     { op(a, b) } -> std::same_as<A>;
};

template <std::regular T>
constexpr T identity_element(std::plus<T> /*op*/) { return T{0U}; }

template <std::regular T>
constexpr T identity_element(std::logical_or<T> /*op*/) { return T{false}; }

template <std::regular T>
constexpr T identity_element(std::multiplies<T> /*op*/) { return T{1U}; }

template <std::regular T>
constexpr T identity_element(std::logical_and<T> /*op*/) { return T{true}; }

template <typename Op, typename A>
concept Monoid = requires(Op op, A a) {
    { Semigroup<Op, A> };
    { identity_element(op) } -> std::same_as<A>;
};

template <std::regular T>
constexpr std::negate<T> inverse_operation(std::plus<T> /*op*/) { return std::negate<T>{}; }

template <std::regular T>
struct reciprocal { constexpr T operator()(const T& x) const { assert(x != T{0U}); return T{1U} / x; } };
template <std::regular T>
constexpr reciprocal<T> inverse_operation(std::multiplies<T> /*op*/) { return reciprocal<T>{}; }

template <typename Op, typename A>
concept Group = requires(Op op, A a) {
    { Monoid<Op, A> };
    { inverse_operation(op) } -> std::same_as<A>;
};

template <typename OpPlus, typename OpTimes, typename A>
concept SemiRing = requires(OpPlus plus, OpTimes times, A a, A b) {
    { plus(a, b) } -> std::same_as<A>;
    { times(a, b) } -> std::same_as<A>;

    { identity_element(plus) } -> std::same_as<A>;
    { identity_element(times) } -> std::same_as<A>;
    { identity_element(plus) != identity_element(times) };

    { plus(a, identity_element(plus)) == a };
    { plus(identity_element(plus), a) == a };

    { times(identity_element(times), a) == a };
    { times(a, identity_element(times)) == a };

    { times(identity_element(plus), a) == identity_element(plus) };
    { times(a, identity_element(plus)) == identity_element(plus) };

    { times(a, plus(a, b)) } -> std::same_as<A>;
    { times(plus(a, b), b) } -> std::same_as<A>;
};
// clang-format on
} // namespace jt::math
