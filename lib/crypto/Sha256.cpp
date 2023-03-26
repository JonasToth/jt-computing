#include "jt-computing/crypto/Sha256.hpp"

#include <algorithm>
#include <bit>
#include <cassert>
#include <iomanip>
#include <sstream>

namespace jt::crypto {

namespace {
std::string str(const std::array<u32, 8> &digest) {
  std::stringstream s;
  s << std::setfill('0') << std::hex;
  auto bytes = std::as_bytes(std::span{digest});
  for (auto b : bytes) {
    // Note: Cast to @c uint is necessary, because @c u8 turns into a char and
    // not a hex-number.
    s << std::setw(2) << std::to_integer<uint>(b);
  }

  return s.str();
}
} // namespace
std::string Sha256Sum::digest() {
  if (_digest.empty()) {
    pad();

    auto hashDigest = std::array<u32, 8>{0};
    if constexpr (std::endian::native == std::endian::little) {
      std::transform(_state.begin(), _state.end(), hashDigest.begin(),
                     [](u32 state) { return std::byteswap(state); });
    } else {
      std::copy(_state.begin(), _state.end(), hashDigest.begin());
    }
    auto digestString = str(hashDigest);
    assert(digestString.size() == 64);
    _digest = digestString;
  }

  return _digest;
}

namespace {
u32 choose(u32 e, u32 f, u32 g) { return (e & f) ^ (~e & g); }
u32 majority(u32 a, u32 b, u32 c) { return (a & (b | c)) | (b & c); }
u32 sig0(u32 x) { return std::rotr(x, 7) ^ std::rotr(x, 18) ^ (x >> 3); }
u32 sig1(u32 x) { return std::rotr(x, 17) ^ std::rotr(x, 19) ^ (x >> 10); }
} // namespace

void Sha256Sum::transform() {
  u32 maj, xorA, ch, xorE, sum, newA, newE;
  std::array<u32, 64> m;
  std::array<u32, 8> state;

  // Split data in 32 bit blocks for the 16 first words
  for (u8 i = 0, j = 0; i < 16; i++, j += 4) {
    m[i] = (_data[j] << 24) | (_data[j + 1] << 16) | (_data[j + 2] << 8) |
           (_data[j + 3]);
  }

  for (u8 k = 16; k < 64; k++) { // Remaining 48 blocks
    m[k] = sig1(m[k - 2]) + m[k - 7] + sig0(m[k - 15]) + m[k - 16];
  }

  for (u8 i = 0; i < 8; ++i) {
    state[i] = _state[i];
  }

  for (u8 i = 0; i < 64; ++i) {
    maj  = majority(state[0], state[1], state[2]);
    xorA = std::rotr(state[0], 2) ^ std::rotr(state[0], 13) ^
           std::rotr(state[0], 22);

    ch   = choose(state[4], state[5], state[6]);

    xorE = std::rotr(state[4], 6) ^ std::rotr(state[4], 11) ^
           std::rotr(state[4], 25);

    sum      = m[i] + Constant[i] + state[7] + ch + xorE;
    newA     = xorA + maj + sum;
    newE     = state[3] + sum;

    state[7] = state[6];
    state[6] = state[5];
    state[5] = state[4];
    state[4] = newE;
    state[3] = state[2];
    state[2] = state[1];
    state[1] = state[0];
    state[0] = newA;
  }

  for (u8 i = 0; i < 8; i++) {
    _state[i] += state[i];
  }
}

void Sha256Sum::pad() {
  // Insert the padding appropriately.
  // The state _must_ include the bit-length of the message at the end with a
  // 64bit unsigned integer.
  // If the already processed message does not leave room for the bit-length,
  // a _new_ block must be done, performing one additional 'transform()'.
  u64 i      = _blockLength;
  u8 end     = _blockLength < 56 ? 56 : 64;

  _data[i++] = 1U << 7U;
  while (i < end) {
    _data[i++] = 0U;
  }

  if (_blockLength >= 56) {
    transform();
    std::fill_n(_data.begin(), 56, 0U);
  }

  // Append to the padding the total message's length in bits and transform.
  _bitLen += _blockLength * 8U;
  _data[63] = static_cast<u8>(_bitLen);
  _data[62] = static_cast<u8>(_bitLen >> 8U);
  _data[61] = static_cast<u8>(_bitLen >> 16U);
  _data[60] = static_cast<u8>(_bitLen >> 24U);
  _data[59] = static_cast<u8>(_bitLen >> 32U);
  _data[58] = static_cast<u8>(_bitLen >> 40U);
  _data[57] = static_cast<u8>(_bitLen >> 48U);
  _data[56] = static_cast<u8>(_bitLen >> 56U);
  transform();
}
void Sha256Sum::reset() {
  // Initialize the state to the constants.
  // clang-format off
  _state = {/*A=*/0x6a09e667, /*B=*/0xbb67ae85,
            /*C=*/0x3c6ef372, /*D=*/0xa54ff53a,
            /*E=*/0x510e527f, /*F=*/0x9b05688c,
            /*G=*/0x1f83d9ab, /*H=*/0x5be0cd19};
  // clang-format on

  // Reset the internal values that are used to pad and process the data.
  _data        = std::array<u8, blockSize>{0};
  _blockLength = 0;
  _bitLen      = 0;
  _digest.clear();
}
} // namespace jt::crypto
