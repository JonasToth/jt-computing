#pragma once

#include "jt-computing/math/Concepts.hpp"
#include <functional>
#include <string>

namespace jt::math {
// clang-format off


template <typename N>
concept NoncommutativeAdditiveSemigroup = requires(N a, N b) {
    { std::regular<N> };
    { a + b } -> std::same_as<N>;
    { a+= b } -> std::same_as<N>;
};

template <typename N>
concept NoncommutativeAdditiveMonoid = requires(N n) {
    { NoncommutativeAdditiveSemigroup<N> };
    { n + N{0U} } -> std::same_as<N>;
    { N{0U} + n } -> std::same_as<N>;
    { n+= N{0U} } -> std::same_as<N>;
};

template <typename N>
concept NoncommutativeAdditiveGroup = requires(N n) {
    { NoncommutativeAdditiveMonoid<N> };
    { n + -n } -> std::same_as<N>;
    { n - n } -> std::same_as<N>;
    { -n + n } -> std::same_as<N>;
    { n+= -n } -> std::same_as<N>;
    { n-= n }  -> std::same_as<N>;
};

template <typename N>
concept MultiplicativeSemigroup = requires(N a, N b) {
    { std::regular<N> };
    { a * b } -> std::same_as<N>;
    { a*= b } -> std::same_as<N>;
};

template <typename N>
concept MultiplicativeMonoid = requires(N n) {
    { MultiplicativeSemigroup<N> };
    { n * N{1U} } -> std::same_as<N>;
    { N{1U} * n } -> std::same_as<N>;
    { n*= N{1U} } -> std::same_as<N>;
};

template <MultiplicativeMonoid A> A multiplicative_inverse(const A &a) {
  return A{1U} / a;
}

template <typename N>
concept MultiplicativeGroup = requires(N n) {
    { MultiplicativeMonoid<N> };
    { n * multiplicative_inverse(n) } -> std::same_as<N>;
    { multiplicative_inverse(n) * n } -> std::same_as<N>;
    { n*= multiplicative_inverse(n) } -> std::same_as<N>;
    { n/= n }  -> std::same_as<N>;
};

template <typename Op, typename A>
concept Semigroup = requires(Op op, A a, A b) {
     { std::regular<A>};
     { op(a, b) } -> std::same_as<A>;
};

template <typename T>
T identity_element(std::plus<T> /*op*/) { return T{0U}; }
template <>
inline std::string identity_element(std::plus<std::string> /*op*/) { return ""; }

template <typename T>
T identity_element(std::multiplies<T> /*op*/) { return T{1U}; }

template <typename Op, typename A>
concept Monoid = requires(Op op, A a) {
    { Semigroup<Op, A> };
    { identity_element(op) } -> std::same_as<A>;
};

template <typename T>
std::negate<T> inverse_operation(std::plus<T> /*op*/) { return std::negate<T>{}; }
template <typename T>
struct reciprocal { T operator()(const T& x) const { assert(x != T{0U}); return T{1U} / x; } };
template <typename T>
reciprocal<T> inverse_operation(std::multiplies<T> /*op*/) { return reciprocal<T>{}; }

template <typename Op, typename A>
concept Group = requires(Op op, A a) {
    { Monoid<Op, A> };
    { inverse_operation(op) } -> std::same_as<A>;
};
// clang-format on

} // namespace jt::math
