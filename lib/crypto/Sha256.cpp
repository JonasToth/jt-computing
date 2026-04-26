export module jt.Crypto:Sha256;

import :Concepts;

import jt.Core;
import std;

namespace {
using namespace jt;

std::string str(const std::array<u32, 8> &digest) {
  std::stringstream s;
  s << std::setfill('0') << std::hex;
  auto bytes = std::as_bytes(std::span{digest});
  for (auto b : bytes) {
    // Note: Cast to @c uint is necessary, because @c u8 turns into a char and
    // not a hex-number.
    s << std::setw(2) << std::to_integer<unsigned int>(b);
  }

  return s.str();
}

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

export namespace jt::crypto {

// Implements Sha256 as described in FIPS PUB 180-4.
class Sha256Sum {
public:
  void process(CryptHashable auto const &data);

  void process(std::string_view str) {
    auto s = std::span{str};
    auto b = std::as_bytes(s);
    return process(b);
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

void Sha256Sum::process(CryptHashable auto const &data) {
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
    // assert(digestString.size() == 64);
    _digest           = digestString;
  }

  return _digest;
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

__attribute__((target("default"))) void Sha256Sum::transform() {
  alignas(cacheLine) std::array<u32, 64> W;

  // Section 6.2.2, Step 1. (1)
  // Insert all data of this block at the start of the message schedule in
  // big-endian words.

  // FIXME: The 'std::span<u32>' code leads to the linker error:
  // undefined symbol: __gnu_cxx::__normal_iterator<unsigned int*, std::span<unsigned int, 18446744073709551615ul>::__iter_tag>::__normal_iterator(unsigned int* const&)
  // after transitioning to modules with clang-21, using libstdc++ on my fedora machine.
#if 1
  u32 const *start = reinterpret_cast<u32 const *>(_data.data());
  // assert(data.size() % sizeof(u32) == 0);
  usize length     = _data.size() / sizeof(u32);

  if constexpr (std::endian::native == std::endian::little) {
    std::transform(start, start + length, W.begin(),
                   [](u32 state) { return std::byteswap(state); });
  } else {
    std::copy(start, start + length, W.begin());
  }
#else
  auto reinterpretedData = std::span<u32>{reinterpret_cast<u32 *>(_data.data()),
                                          blockSize / sizeof(u32)};

  if constexpr (std::endian::native == std::endian::little) {
    std::transform(reinterpretedData.begin(), reinterpretedData.end(),
                   W.begin(), [](u32 state) { return std::byteswap(state); });
  } else {
    std::copy(reinterpretedData.begin(), reinterpretedData.end(), W.begin());
  }
#endif

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

#if 0
__attribute__((target("sse4.2"))) void Sha256Sum::transform() {
  // Initially Copied from
  // https://github.com/randombit/botan/blob/master/src/lib/hash/sha2_32/sha2_32_x86/sha2_32_x86.cpp
  // Botans implementation.
  const __m128i *K_mm     = reinterpret_cast<const __m128i *>(K.data());
  const __m128i *input_mm = reinterpret_cast<const __m128i *>(_data.data());
  const __m128i MASK = _mm_set_epi64x(0x0c0d0e0f08090a0b, 0x0405060700010203);

  // Load initial digest state via _aligned_ load instructions.
  __m128i STATE0     = _mm_load_si128(reinterpret_cast<__m128i *>(H.data()));
  __m128i STATE1    = _mm_load_si128(reinterpret_cast<__m128i *>(H.data() + 4));

  STATE0            = _mm_shuffle_epi32(STATE0, 0xB1); // CDAB
  STATE1            = _mm_shuffle_epi32(STATE1, 0x1B); // EFGH

  const __m128i TMP = _mm_alignr_epi8(STATE0, STATE1, 8);    // ABEF
  STATE1            = _mm_blend_epi16(STATE1, STATE0, 0xF0); // CDGH
  STATE0            = TMP;

  {
    const __m128i ABEF_SAVE = STATE0;
    const __m128i CDGH_SAVE = STATE1;

    __m128i MSG;

    __m128i TMSG0 = _mm_shuffle_epi8(_mm_loadu_si128(input_mm), MASK);
    __m128i TMSG1 = _mm_shuffle_epi8(_mm_loadu_si128(input_mm + 1), MASK);
    __m128i TMSG2 = _mm_shuffle_epi8(_mm_loadu_si128(input_mm + 2), MASK);
    __m128i TMSG3 = _mm_shuffle_epi8(_mm_loadu_si128(input_mm + 3), MASK);

    // Rounds 0-3
    MSG           = _mm_add_epi32(TMSG0, _mm_load_si128(K_mm));
    STATE1        = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    // Rounds 4-7
    MSG    = _mm_add_epi32(TMSG1, _mm_load_si128(K_mm + 1));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG0  = _mm_sha256msg1_epu32(TMSG0, TMSG1);

    // Rounds 8-11
    MSG    = _mm_add_epi32(TMSG2, _mm_load_si128(K_mm + 2));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG1  = _mm_sha256msg1_epu32(TMSG1, TMSG2);

    // Rounds 12-15
    MSG    = _mm_add_epi32(TMSG3, _mm_load_si128(K_mm + 3));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG0  = _mm_add_epi32(TMSG0, _mm_alignr_epi8(TMSG3, TMSG2, 4));
    TMSG0  = _mm_sha256msg2_epu32(TMSG0, TMSG3);
    TMSG2  = _mm_sha256msg1_epu32(TMSG2, TMSG3);

    // Rounds 16-19
    MSG    = _mm_add_epi32(TMSG0, _mm_load_si128(K_mm + 4));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG1  = _mm_add_epi32(TMSG1, _mm_alignr_epi8(TMSG0, TMSG3, 4));
    TMSG1  = _mm_sha256msg2_epu32(TMSG1, TMSG0);
    TMSG3  = _mm_sha256msg1_epu32(TMSG3, TMSG0);

    // Rounds 20-23
    MSG    = _mm_add_epi32(TMSG1, _mm_load_si128(K_mm + 5));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG2  = _mm_add_epi32(TMSG2, _mm_alignr_epi8(TMSG1, TMSG0, 4));
    TMSG2  = _mm_sha256msg2_epu32(TMSG2, TMSG1);
    TMSG0  = _mm_sha256msg1_epu32(TMSG0, TMSG1);

    // Rounds 24-27
    MSG    = _mm_add_epi32(TMSG2, _mm_load_si128(K_mm + 6));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG3  = _mm_add_epi32(TMSG3, _mm_alignr_epi8(TMSG2, TMSG1, 4));
    TMSG3  = _mm_sha256msg2_epu32(TMSG3, TMSG2);
    TMSG1  = _mm_sha256msg1_epu32(TMSG1, TMSG2);

    // Rounds 28-31
    MSG    = _mm_add_epi32(TMSG3, _mm_load_si128(K_mm + 7));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG0  = _mm_add_epi32(TMSG0, _mm_alignr_epi8(TMSG3, TMSG2, 4));
    TMSG0  = _mm_sha256msg2_epu32(TMSG0, TMSG3);
    TMSG2  = _mm_sha256msg1_epu32(TMSG2, TMSG3);

    // Rounds 32-35
    MSG    = _mm_add_epi32(TMSG0, _mm_load_si128(K_mm + 8));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG1  = _mm_add_epi32(TMSG1, _mm_alignr_epi8(TMSG0, TMSG3, 4));
    TMSG1  = _mm_sha256msg2_epu32(TMSG1, TMSG0);
    TMSG3  = _mm_sha256msg1_epu32(TMSG3, TMSG0);

    // Rounds 36-39
    MSG    = _mm_add_epi32(TMSG1, _mm_load_si128(K_mm + 9));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG2  = _mm_add_epi32(TMSG2, _mm_alignr_epi8(TMSG1, TMSG0, 4));
    TMSG2  = _mm_sha256msg2_epu32(TMSG2, TMSG1);
    TMSG0  = _mm_sha256msg1_epu32(TMSG0, TMSG1);

    // Rounds 40-43
    MSG    = _mm_add_epi32(TMSG2, _mm_load_si128(K_mm + 10));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG3  = _mm_add_epi32(TMSG3, _mm_alignr_epi8(TMSG2, TMSG1, 4));
    TMSG3  = _mm_sha256msg2_epu32(TMSG3, TMSG2);
    TMSG1  = _mm_sha256msg1_epu32(TMSG1, TMSG2);

    // Rounds 44-47
    MSG    = _mm_add_epi32(TMSG3, _mm_load_si128(K_mm + 11));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG0  = _mm_add_epi32(TMSG0, _mm_alignr_epi8(TMSG3, TMSG2, 4));
    TMSG0  = _mm_sha256msg2_epu32(TMSG0, TMSG3);
    TMSG2  = _mm_sha256msg1_epu32(TMSG2, TMSG3);

    // Rounds 48-51
    MSG    = _mm_add_epi32(TMSG0, _mm_load_si128(K_mm + 12));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG1  = _mm_add_epi32(TMSG1, _mm_alignr_epi8(TMSG0, TMSG3, 4));
    TMSG1  = _mm_sha256msg2_epu32(TMSG1, TMSG0);
    TMSG3  = _mm_sha256msg1_epu32(TMSG3, TMSG0);

    // Rounds 52-55
    MSG    = _mm_add_epi32(TMSG1, _mm_load_si128(K_mm + 13));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG2  = _mm_add_epi32(TMSG2, _mm_alignr_epi8(TMSG1, TMSG0, 4));
    TMSG2  = _mm_sha256msg2_epu32(TMSG2, TMSG1);

    // Rounds 56-59
    MSG    = _mm_add_epi32(TMSG2, _mm_load_si128(K_mm + 14));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    TMSG3  = _mm_add_epi32(TMSG3, _mm_alignr_epi8(TMSG2, TMSG1, 4));
    TMSG3  = _mm_sha256msg2_epu32(TMSG3, TMSG2);

    // Rounds 60-63
    MSG    = _mm_add_epi32(TMSG3, _mm_load_si128(K_mm + 15));
    STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);
    STATE0 =
        _mm_sha256rnds2_epu32(STATE0, STATE1, _mm_shuffle_epi32(MSG, 0x0E));

    // Add values back to state
    STATE0 = _mm_add_epi32(STATE0, ABEF_SAVE);
    STATE1 = _mm_add_epi32(STATE1, CDGH_SAVE);
  }

  STATE0 = _mm_shuffle_epi32(STATE0, 0x1B); // FEBA
  STATE1 = _mm_shuffle_epi32(STATE1, 0xB1); // DCHG

  // Save state
  _mm_storeu_si128(reinterpret_cast<__m128i *>(H.data()),
                   _mm_blend_epi16(STATE0, STATE1, 0xF0)); // DCBA
  _mm_storeu_si128(reinterpret_cast<__m128i *>(H.data() + 4),
                   _mm_alignr_epi8(STATE1, STATE0, 8)); // ABEF
}
#endif
} // namespace jt::crypto
