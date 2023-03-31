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
      std::transform(H.begin(), H.end(), hashDigest.begin(),
                     [](u32 state) { return std::byteswap(state); });
    } else {
      std::copy(H.begin(), H.end(), hashDigest.begin());
    }
    auto digestString = str(hashDigest);
    assert(digestString.size() == 64);
    _digest = digestString;
  }

  return _digest;
}

namespace {
using std::rotr;
/// Defined in Section 4.1.2, (4.2).
u32 Ch(u32 x, u32 y, u32 z) { return (x & y) ^ (~x & z); }
/// Defined in Section 4.1.2, (4.3).
u32 Maj(u32 x, u32 y, u32 z) { return (x & y) ^ (x & z) ^ (y & z); }
/// Defined in Section 4.1.2, (4.4).
u32 Sum0(u32 x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }
/// Defined in Section 4.1.2, (4.5).
u32 Sum1(u32 x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
/// Defined in Section 4.1.2, (4.6).
u32 Sig0(u32 x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3U); }
/// Defined in Section 4.1.2, (4.7).
u32 Sig1(u32 x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10U); }
} // namespace

void Sha256Sum::transform() {
  alignas(cacheLine) std::array<u32, 64> W;

  // Section 6.2.2, Step 1. (1)
  // Insert all data of this block at the start of the message schedule in
  // big-endian words.
  auto dataReinterpreted = std::span<u32>{reinterpret_cast<u32 *>(_data.data()),
                                          blockSize / sizeof(u32)};
  if constexpr (std::endian::native == std::endian::little) {
    std::transform(dataReinterpreted.begin(), dataReinterpreted.end(),
                   W.begin(), [](u32 state) { return std::byteswap(state); });
  } else {
    std::copy(dataReinterpreted.begin(), dataReinterpreted.end(), W.begin());
  }

  // Section 6.2.2, Step 1. (2).
  // The rest of the message schedule is derived from the previous data words.
  for (u8 t = 16; t < 64; ++t) {
    W[t] = Sig1(W[t - 2]) + W[t - 7] + Sig0(W[t - 15]) + W[t - 16];
  }

  // Section 6.2.2, Step 2.
  alignas(cacheLine) std::array<u32, 8> abcdefgh = H;
  auto &[a, b, c, d, e, f, g, h]                 = abcdefgh;

  // Section 6.2.2, Step 3.
  for (u8 t = 0; t < 64; ++t) {
    // clang-format off
    u32 T1 =   h
             + Sum1(e)
             + Ch(e, f, g)
             + K[t]
             + W[t];
    u32 T2 = Sum0(a) + Maj(a, b, c);
    // clang-format on

    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }

  for (u8 i = 0; i < 8; i++) {
    H[i] += abcdefgh[i];
  }
}

void Sha256Sum::pad() {
  // Defined in Section 5.1.1.
  //
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
  // Defined in 5.3.3.
  //
  // Initialize the state to the constants.
  // clang-format off
  H = {/*A=*/0x6a09e667, /*B=*/0xbb67ae85,
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
