export module jt.Crypto:TextbookRSA;

import std;
import jt.Math;

using namespace std;

export namespace jt::crypto {

enum class Key {
  Private, ///< Private Key in RSA.
  Public,  ///< Public Key in RSA.
};

template <Key K, math::NaturalNumber N = math::NaturalN> class TextbookRSA {
public:
  TextbookRSA(N modul, N e) : _modulus{move(modul)}, _e{move(e)} {}

  [[nodiscard]] N apply(N plainText) const {
    return math::power_monoid(plainText, _e, math::multiplies_mod{_modulus});
  }

private:
  N _modulus;
  N _e;
};

// See "PKCS #1 V2.2: RSA - Section 9.2 EMSA-PKCS1-v1_5"
constexpr string_view PKCS1V1_5SignaturePadding{
    // Padding bytes.
    "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
    "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
    "fffffffff00"
    // ASN.1 Encoding for the Algorithm ID. Defined in Note (1).
    "3031300d060960864801650304020105000420"};

template <math::NaturalNumber N = math::NaturalN> class RSASha256Signing {
public:
  explicit RSASha256Signing(TextbookRSA<Key::Private, N> key)
      : _key{move(key)} {}

  [[nodiscard]] string signEMSA_PKCS1v1_5(string_view sha256Hash) const {
    if (sha256Hash.size() != 64) {
      throw invalid_argument{"invalid length for sha256 hash"};
    }
    string rawSignaturePadded{PKCS1V1_5SignaturePadding};
    rawSignaturePadded += sha256Hash;

    auto signature = N{0U};
    istringstream{rawSignaturePadded} >> hex >> signature;

    const auto encrypted = _key.apply(signature);

    ostringstream oss;
    oss << hex << encrypted;

    return oss.str();
  }

private:
  TextbookRSA<Key::Private, N> _key;
};

template <math::NaturalNumber N = math::NaturalN>
class RSASha256SigVerification {
public:
  explicit RSASha256SigVerification(TextbookRSA<Key::Public, N> key)
      : _key{move(key)} {}

  [[nodiscard]] bool verifyEMSA_PKCS1v1_5(string_view signature,
                                          string_view sha256Hash) const {
    if (sha256Hash.size() != 64) {
      throw invalid_argument{"invalid length for sha256 hash"};
    }
    string expectedSigStr{PKCS1V1_5SignaturePadding};
    expectedSigStr += sha256Hash;

    auto expectedSignature = N{0U};
    istringstream{expectedSigStr} >> hex >> expectedSignature;

    auto providedSignature = N{0U};
    istringstream{string{signature}} >> hex >> providedSignature;

    const auto decrypted = _key.apply(providedSignature);
    return expectedSignature == decrypted;
  }

private:
  TextbookRSA<Key::Public, N> _key;
};
} // namespace jt::crypto
