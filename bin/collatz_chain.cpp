#include "jt-computing/math/BigUInt.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace jt;
using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    assert(argc >= 1);
    std::cerr << "Usage: " << argv[0] << "\n\n"
              << "Determines the length of the collatz-row of <n> until '1' is "
                 "reached.\n";
    return EXIT_FAILURE;
  }

  assert(argc >= 2);

  std::stringstream ss(argv[1]);
  math::BigUInt n{0U};
  ss >> n;

  std::cout << "Finding the length of collatz chain for '" << n << "'"
            << std::endl;

  math::BigUInt chainLength{0U};
  math::BigUInt peak{1U};

  while (n != 1U) {
    if (n.isEven()) {
      // n = n / 2
      n >>= 1U;
    } else {
      // n = 3n + 1
      n *= 3U;
      n += 1U;
    }
    chainLength += 1U;
    peak = std::max(peak, n);
  }

  std::cout << "'" << n << "' took '" << chainLength << "' steps to reach 1."
            << std::endl;
  std::cout << "The highest number in the chain is '" << peak << "'."
            << std::endl;

  return EXIT_SUCCESS;
}
