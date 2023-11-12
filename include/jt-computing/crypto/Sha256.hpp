#pragma once

#include "jt-computing/Types.hpp"
#include "jt-computing/crypto/Concepts.hpp"

#include <array>
#include <bit>
#include <ranges>
#include <span>
#include <string>

namespace jt::crypto {

// Implements Sha256 as described in FIPS PUB 180-4.
class Sha256Sum {
public:
  void process(CryptHashable auto data);

  void process(std::string_view str) {
    return process(std::as_bytes(std::span{str}));
  }

  template <std::input_iterator I> void process(I begin, I end) {
    return process(std::ranges::subrange{begin, end});
  }

  std::string digest();
  void reset();

private:
  static constexpr usize blockSize = 64;
  static constexpr auto cacheLine  = 64;
  alignas(cacheLine) std::array<u8, blockSize> _data{0};

  /// Defined in Section 5.3.3.
  alignas(cacheLine) std::array<u32, 8> H{
      /*A=*/0x6a09e667, /*B=*/0xbb67ae85, /*C=*/0x3c6ef372,
      /*D=*/0xa54ff53a, /*E=*/0x510e527f, /*F=*/0x9b05688c,
      /*G=*/0x1f83d9ab, /*H=*/0x5be0cd19};

  // Constants defined in Section 4.2.3.
  alignas(cacheLine) static constexpr std::array<u32, 64> K = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
      0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

  u64 _blockLength{0};
  u64 _bitLen{0};
  std::string _digest;

  __attribute__((target("default"))) void transform();
#if 0
  __attribute__((target("sse4.2"))) void transform();
#endif
  void pad();
};

void Sha256Sum::process(CryptHashable auto data) {
  if (!_digest.empty()) {
    throw std::runtime_error{"Digest Computed, Can not further update Message"};
  }
  for (auto b : data) {
    _data[_blockLength++] = std::bit_cast<u8>(b);

    // Once a datablock is full, apply the "compression function" that actually
    // hashes.
    if (_blockLength == blockSize) {
      transform();

      // End of the block
      _bitLen += blockSize * 8U;
      _blockLength = 0;
    }
  }
}
} // namespace jt::crypto
