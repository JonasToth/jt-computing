#include "jt-computing/math/RandomPrime.hpp"

#include <random>

namespace jt::math {
NaturalN generateRandomNumber(i32 bits) {
  std::random_device rd;
  constexpr i32 bitsPerIteration = sizeof(std::random_device::result_type);

  auto result                    = 1_U;
  --bits;

  for (; bits > bitsPerIteration; bits -= bitsPerIteration) {
    result <<= bitsPerIteration;
    result += NaturalN{rd()};
  }

  return result;
}
} // namespace jt::math
