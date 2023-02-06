#ifndef NATURAL_NUMBER_ALGORITHMS_12ASDBD
#define NATURAL_NUMBER_ALGORITHMS_12ASDBD

#include "jt-computing/Types.hpp"
#include "jt-computing/container/BitVector.hpp"
#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/Concepts.hpp"

#include <cmath>
#include <functional>
#include <vector>

namespace jt::math {

/// Computes all prime factors that are not @c 1 or @c n and returns them.
/// @returns sorted vector of prime factors, starting with the smallest.
/// @code
/// const auto factors = getPrimeFactors(84U);
/// factors == {2, 2, 3, 7};
/// @endcode
template <NaturalNumber N> std::vector<N> getPrimeFactors(N n);

/// Checks if a number is prime by determining its prime factors. If the number
/// is even, it returns early with a @c false.
/// @sa getPrimeFactors
template <NaturalNumber N> bool isPrime(NaturalNumber auto n);

/// Computes the prime numbers up to a specific N.
/// @warning Uses O(N) ~ memory for the computation.
/// @returns vector of prime numbers.
template <NaturalNumber N>
std::vector<N> sieveEratosthenes(const usize &maximum);

/// Computes the greatest-common-divisor for natural numbers.
template <NaturalNumber N> N gcd(N a, N b);

/// Computes the least-common-multiple for natural numbers.
template <NaturalNumber N> N lcm(const N &a, const N &b);

/// Algorithm for exponentiation of natural numbers.
template <NaturalNumber N, typename MultiplicativeFunction = std::multiplies<N>>
N square_multiply(N a, N b, MultiplicativeFunction F = {});

template <NaturalNumber N> std::vector<N> getPrimeFactors(N n) {
  std::vector<N> result;

  if (n == N{0U} || n == N{1U} || n == N{2U}) {
    return result;
  }

  N originalN{n};
  N currentDivisor{2U};

  // Try to divide n by the @c currentDivisor. If the division is possible
  // without remainder, the @c currentDivisor is a prime factor and must be
  // stored.
  // The division must be repeated multiple times, until it fails, because
  // a number may have a prime factor mulitple times.
  // E.g. 4 == 2 * 2.
  while (currentDivisor < originalN) {
    const auto [quotient, remainder] = divmod(n, currentDivisor);
    if (remainder == N{0U}) {
      result.push_back(currentDivisor);

      // The algorithm is done. The final calculation is a perfect division.
      // E.g.
      // 15 / 2 = 7, 1
      // 15 / 3 = 5, 0 -> store 3
      //  5 / 5 = 1, 0 -> store 5; terminate
      if (quotient == N{1U}) {
        break;
      }

      // Repeat the divison, now with one prime factor already removed from the
      // number. The @c currentDivisor shall _not_ be increased yet.
      // Think of 'n == 4'!
      n = quotient;
      continue;
    }
    // Advance to 2 -> 3 for the divisor at the beginning. For values starting
    // from 3, increment by two, because even values can not be prime factors,
    // except for 2.
    currentDivisor += (currentDivisor == N{2U} ? N{1U} : N{2U});
  }
  return result;
}

template <NaturalNumber N> bool isPrime(N n) {
  if (n == N{0U} || n == N{1U}) {
    return false;
  }
  if (n == N{2U}) {
    return true;
  }

  if constexpr (std::is_same_v<N, BigUInt>) {
    if (n.isEven()) {
      return false;
    }
  } else {
    if (n % N{2U} == N{0U}) {
      return false;
    }
  }

  N halfN{n / N{2U}};
  N currentDivisor{3U};

  while (currentDivisor < halfN) {
    const auto remainder = n % currentDivisor;
    if (remainder == N{0U}) {
      return false;
    }
    // Increments by 2, because it is not necessary to check even numbers.
    currentDivisor += N{2U};
  }
  return true;
}

template <NaturalNumber N>
std::vector<N> sieveEratosthenes(const usize &maximum) {
  // All numbers are potentially prime numbers at the beginning. The algorithm
  // strikes through specific numbers.
  container::BitVector sieve{maximum, /*initialValue=*/true};

  // This vector stores the determined primes.
  std::vector<N> collectedPrimes;

  // The number 0 and 1 are not prime numbers.
  sieve.set(0, false);
  sieve.set(1, false);

  // Start striking out with 2U ...
  usize currentMinimalFactor{2U};
  // ... and stop at sqrt(maximum). At this point, all prime numbers are
  // discovered.
  usize maximalFactor{static_cast<usize>(std::ceil(std::sqrt(maximum)))};

  while (currentMinimalFactor <= maximalFactor) {
    collectedPrimes.emplace_back(N{currentMinimalFactor});

    // Strike out all multiples of the current prime number.
    for (usize i = 2U; i < maximum; ++i) {
      usize strikeOutIndex = currentMinimalFactor * i;
      if (strikeOutIndex >= maximum) {
        break;
      }
      sieve.set(strikeOutIndex, false);
    }

    // Search for the next not-striked out index in the sieve. This is a prime
    // number, that will be used for the next iteration.
    const auto findNextPrime = [&sieve, &maximum](usize currentMinimalPrime) {
      for (usize i = currentMinimalPrime + usize{1U}; i < maximum; i++) {
        if (sieve.get(i)) {
          return i;
        }
      }
      throw std::runtime_error{"Failed to determine next prime number."
                               " It should terminate!"};
    };
    currentMinimalFactor = findNextPrime(currentMinimalFactor);
  }

  // Collect all primes that are left from the sieve.
  for (usize i = currentMinimalFactor; i < maximum; i += usize{2U}) {
    if (sieve.get(i)) {
      collectedPrimes.emplace_back(N{i});
    }
  }

  return collectedPrimes;
}

template <NaturalNumber N> N gcd(N a, N b) {
  while (b != N{0U}) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

template <NaturalNumber N> N lcm(const N &a, const N &b) {
  const auto g      = gcd(a, b);
  const auto p      = a * b;
  const auto result = p / g;
  return result;
}
#include <iostream>
template <NaturalNumber N, typename MultiplicativeFunction>
N square_multiply(N a, N b, MultiplicativeFunction F) {
  auto accumulator = N{1U};
  if (a == N{0U}) {
    return accumulator;
  }
  while (b > N{0U}) {
    if (isOdd(b)) {
      accumulator = F(accumulator, a);
    }
    a = F(a, a);
    b /= N{2U};
  }
  return accumulator;
}
} // namespace jt::math

#endif
