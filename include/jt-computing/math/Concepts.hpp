#ifndef MATH_CONCEPTS_14ASLDJ
#define MATH_CONCEPTS_14ASLDJ

#include <concepts>

namespace jt::math {

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
                         std::equality_comparable<N> &&
                         std::copy_constructible<N> &&
                         std::totally_ordered<N> &&
                         !std::signed_integral<N>);
// clang-format on

} // namespace jt::math

#endif
