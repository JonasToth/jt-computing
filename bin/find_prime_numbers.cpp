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
    return EXIT_FAILURE;
  }

  assert(argc >= 2);
  usize maximumNumber{std::stoull(argv[1])};

  std::cout << "Finding all primes up to " << maximumNumber << std::endl;

  const auto primes = math::sieveEratosthenes<usize>(maximumNumber);

  for (const auto prime : primes) {
    std::cout << prime << "\n";
  }

  return EXIT_SUCCESS;
}
