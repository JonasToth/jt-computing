#pragma once

#include <climits>
#include <ranges>

namespace jt::crypto {

static_assert(CHAR_BIT == 8, "Bytes are made of 8 Bits");
template <typename T>
concept ByteSizedValueType = requires(T t) {
  { sizeof(typename T::value_type) == 1 };
};
template <typename T>
concept CryptHashable = std::ranges::forward_range<T> && ByteSizedValueType<T>;

template <typename T>
concept HashFunctor = requires(T t) {
  { t.process("Hello World") } -> std::same_as<void>;
  { t.digest() } -> std::same_as<std::string>;
  { t.reset() } -> std::same_as<void>;
};

} // namespace jt::crypto
