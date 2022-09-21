#include "jt-computing/math/NaturalNumberAlgorithms.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace jt;
using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    assert(argc >= 1);
    std::cerr << "Usage: " << argv[0] << "\n\n"
              << "Finds the first <n> prime numbers using the sieve of "
                 "eratosthenes. Each prime is printed on its own line\n";
  }

  assert(argc >= 2);
  usize maximumNumber{std::stoull(argv[1])};

  const auto primes = math::sieveEratosthenes<usize>(maximumNumber);

  for (const auto prime : primes) {
    std::cout << prime << "\n";
  }

  return 0;
}
