export module jt.Math:Concepts;

import std;

// -- Algebra Concepts --
export namespace jt::math {
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
struct reciprocal {
    constexpr T operator()(const T& x) const {
        // assert(x != T{0U});
        return T{1U} / x;
    }
};
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

// -- Number Concepts --

export namespace jt::math {

// clang-format off
template <typename N>
concept Computable = requires(N a, N b, N c) {
  { a + b } -> std::same_as<N>;
  { a - b } -> std::same_as<N>;
  { a * b } -> std::same_as<N>;
  { a / b } -> std::same_as<N>;
  { a % b } -> std::same_as<N>;

  { c += b } -> std::same_as<N &>;
  { c -= b } -> std::same_as<N &>;
  { c *= b } -> std::same_as<N &>;
  { c /= b } -> std::same_as<N &>;
  { c %= b } -> std::same_as<N &>;
};

template <typename N>
concept NaturalNumber = std::unsigned_integral<N> ||
                        (Computable<N> &&
                         std::regular<N> &&
                         std::totally_ordered<N> &&
                         !std::signed_integral<N>);

template <typename N>
concept Integer = (std::unsigned_integral<N> || std::signed_integral<N> || NaturalNumber<N>) && 
                  Computable<N> && std::regular<N> && std::totally_ordered<N>;

// clang-format on

} // namespace jt::math
