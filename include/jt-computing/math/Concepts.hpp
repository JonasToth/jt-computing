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
                         std::regular<N> &&
                         std::totally_ordered<N> &&
                         !std::signed_integral<N>);

template <typename N>
concept Integer = (std::unsigned_integral<N> || std::signed_integral<N> || NaturalNumber<N>) && 
                  Computable<N> && std::regular<N> && std::totally_ordered<N>;

// clang-format on

} // namespace jt::math

#endif
