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
T identity_element(std::plus<T> /*op*/) { return T{0U}; }

template <std::regular T>
T identity_element(std::multiplies<T> /*op*/) { return T{1U}; }

template <typename Op, typename A>
concept Monoid = requires(Op op, A a) {
    { Semigroup<Op, A> };
    { identity_element(op) } -> std::same_as<A>;
};

template <std::regular T>
std::negate<T> inverse_operation(std::plus<T> /*op*/) { return std::negate<T>{}; }

template <std::regular T>
struct reciprocal { T operator()(const T& x) const { assert(x != T{0U}); return T{1U} / x; } };
template <std::regular T>
reciprocal<T> inverse_operation(std::multiplies<T> /*op*/) { return reciprocal<T>{}; }

template <typename Op, typename A>
concept Group = requires(Op op, A a) {
    { Monoid<Op, A> };
    { inverse_operation(op) } -> std::same_as<A>;
};
// clang-format on

} // namespace jt::math
