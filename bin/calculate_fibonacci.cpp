#include "jt-computing/math/BigUInt.hpp"

#include <cassert>
#include <iostream>
#include <unordered_map>

using namespace jt;

math::BigUInt fibbonacci(usize n) {
  static std::vector<math::BigUInt> memoized{math::BigUInt{0U},
                                             math::BigUInt{1U}};
  if (n == 0) {
    return memoized[0];
  }
  if (n == 1) {
    return memoized[1];
  }

  if (n >= memoized.size()) {
    const usize oldSize = memoized.size();
    memoized.resize(n + 1, math::BigUInt{0U});

    for (usize i = oldSize; i <= n; ++i) {
      memoized[i] += memoized.at(i - 2);
      memoized[i] += memoized.at(i - 1);
    }
  }
  return memoized.at(n);
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    assert(argc >= 1);
    std::cerr << "Usage: " << argv[0] << "\n\n"
              << "Computes the 10th fibonacci number by plain recursion and "
                 "assert its result.\n";
    return EXIT_FAILURE;
  }
  const auto fib0      = fibbonacci(0U);
  const auto expected0 = math::BigUInt{0ULL};
  std::cout << "fib(0) = 0? -> " << (fib0 == expected0 ? "t" : "f") << "\n";

  const auto fib1      = fibbonacci(1U);
  const auto expected1 = math::BigUInt{1ULL};
  std::cout << "fib(1) = 1? -> " << (fib1 == expected1 ? "t" : "f") << "\n";

  const auto fib2      = fibbonacci(2U);
  const auto expected2 = math::BigUInt{1ULL};
  std::cout << "fib(2) = 1? -> " << (fib2 == expected2 ? "t" : "f") << "\n";

  const auto fib3      = fibbonacci(3U);
  const auto expected3 = math::BigUInt{2ULL};
  std::cout << "fib(3) = 2? -> " << (fib3 == expected3 ? "t" : "f") << "\n";

  const auto fib5      = fibbonacci(5U);
  const auto expected5 = math::BigUInt{5ULL};
  std::cout << "fib(5) = 5? -> " << (fib5 == expected5 ? "t" : "f") << "\n";

  const auto fib6      = fibbonacci(6U);
  const auto expected6 = math::BigUInt{8ULL};
  std::cout << "fib(6) = 8? -> " << (fib6 == expected6 ? "t" : "f") << "\n";

  const auto fib7      = fibbonacci(7U);
  const auto expected7 = math::BigUInt{13ULL};
  std::cout << "fib(7) = 13? -> " << (fib7 == expected7 ? "t" : "f") << "\n";

  const auto fib10      = fibbonacci(10U);
  const auto expected10 = math::BigUInt{55ULL};
  std::cout << "fib(10) = 55? -> " << (fib10 == expected10 ? "t" : "f") << "\n";

  const auto fib61      = fibbonacci(61U);
  const auto expected61 = math::BigUInt{2504730781961ULL};
  std::cout << "fib(61) = 2504730781961? -> "
            << (fib61 == expected61 ? "t" : "f") << "\n";

  return EXIT_SUCCESS;
}
