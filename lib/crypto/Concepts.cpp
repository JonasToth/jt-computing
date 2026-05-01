export module jt.Crypto:Concepts;

import std;
using namespace std;

export namespace jt::crypto {

template <typename T>
concept ByteSizedValueType = requires() {
  { sizeof(typename ranges::range_value_t<T>) == 1 };
};
template <typename T>
concept ByteSizedIterator = requires() {
  { sizeof(typename T::value_type) == 1 };
};
template <typename T>
concept CryptHashable = ranges::input_range<T> && ByteSizedValueType<T>;

template <typename T>
concept HashFunctor = requires(T t) {
  { t.process("Hello World") } -> same_as<void>;
  { t.digest() } -> same_as<string>;
  { t.reset() } -> same_as<void>;
};

} // namespace jt::crypto
