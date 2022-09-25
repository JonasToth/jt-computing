#include "jt-computing/math/NaturalNumberAlgorithms.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace std;
using namespace jt;
using namespace jt::math;

int main(int argc, char **argv) {
  if (argc != 2) {
    assert(argc >= 1);
    cerr << "Usage: " << argv[0] << " <n>\n\n"
         << "Finds the first <n> prime numbers using the sieve of "
            "eratosthenes. Each prime is printed on its own line\n";
    return EXIT_FAILURE;
  }

  assert(argc >= 2);
  usize maximumNumber{stoull(argv[1])};

  cout << "Finding all primes up to " << maximumNumber << endl;
  for (const auto &prime : sieveEratosthenes<BigUInt>(maximumNumber)) {
    cout << prime << "\n";
  }

  return EXIT_SUCCESS;
}
