#include "jt-computing/math/BigUInt.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace jt;
using namespace jt::math;
using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    assert(argc >= 1);
    cerr << "Usage: " << argv[0] << " <n>\n\n"
         << "Determines the length of the collatz-row of <n> until '1' is "
            "reached.\n";
    return EXIT_FAILURE;
  }

  assert(argc >= 2);

  stringstream ss(argv[1]);
  auto n = 0_N;
  ss >> n;

  cout << "It takes '" << n << "' exactly";

  auto chainLength = 0_N;
  auto peak        = 1_N;

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
    peak = max(peak, n);
  }

  cout << " '" << chainLength << "' steps to reach 1." << endl;
  cout << "The highest number in the chain is '" << peak << "'." << endl;

  return EXIT_SUCCESS;
}
