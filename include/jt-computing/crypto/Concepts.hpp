#pragma once

#include <climits>
#include <ranges>
#include <type_traits>

namespace jt::crypto {

static_assert(CHAR_BIT == 8, "Bytes are made of 8 Bits");
template <typename T>
concept ByteSizedValueType = requires() {
  { sizeof(typename std::ranges::range_value_t<T>) == 1 };
};
template <typename T>
concept ByteSizedIterator = requires() {
  { sizeof(typename T::value_type) == 1 };
};
template <typename T>
concept CryptHashable = std::ranges::input_range<T> && ByteSizedValueType<T>;

template <typename T>
concept HashFunctor = requires(T t) {
  { t.process("Hello World") } -> std::same_as<void>;
  { t.digest() } -> std::same_as<std::string>;
  { t.reset() } -> std::same_as<void>;
};

} // namespace jt::crypto
