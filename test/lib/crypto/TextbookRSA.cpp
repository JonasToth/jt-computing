#include "jt-computing/crypto/TextbookRSA.hpp"
#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/NaturalN.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace std;
using namespace std::string_literals;

namespace {
const auto modul =
    "C32F20E0AB05983D8AB9FD8FCC85CDFB6DFBA49D4E5A17A543486B03EA55D1B09CA6C825"
    "030C7D7F57F0D56092648DB4255C339CCAD83AD5D391DBEAD94E6420E2D78B981954CEE3"
    "B0FAFF7ECC42E39DE698EEA2AD1EDE6D46CEA6D6737A5D92B6028D06DFF3B381190F4266"
    "EB818E4AC7C3400F139B2261AB6B1B9438FEB98F"s;
const auto public_exponent = "010001"s;
const auto private_exponent =
    "BCF421FAFDF5B395587887ED2F0D1604E34B6AE85292CB33553885494C117AA7646FDD70"
    "5F4C372E119F77A23EF6936C2382E800512B15822D889602FC0E74C4E24D86637B4AB78A"
    "AB76F1DF31EDDAE06B45C675B62976392E813C54B200C5D6C0397B187862FC67851B6C1C"
    "25EF1703A8863B32EBEF41833637014E388FB5C1"s;

template <typename Int> Int fromHexString(const std::string &numberHex) {
  Int result{0U};
  std::istringstream{numberHex} >> std::hex >> result;
  return result;
}
template <typename Int> std::string toHexString(const Int &n) {
  std::ostringstream oss;
  oss << std::hex << n;
  return oss.str();
}
} // namespace

TEMPLATE_TEST_CASE("TextbookRSA Encrypt/Decrypt Inversion", "", math::BigUInt,
                   math::NaturalN) {
  auto n_modulus          = fromHexString<TestType>(modul);
  auto n_public_exponent  = fromHexString<TestType>(public_exponent);
  auto n_private_exponent = fromHexString<TestType>(private_exponent);

  auto rsaPub = crypto::TextbookRSAPublic{n_modulus, n_public_exponent};
  auto rsaPrv = crypto::TextbookRSAPrivate{n_modulus, n_private_exponent};

  const auto hash_document =
      "abe6fe6030068e1f1e7c72c7aad54b77247b48e386a50cdc556a36ec5986d135"s;
  auto n_hash = fromHexString<TestType>(hash_document);

  REQUIRE(rsaPrv.decrypt(rsaPub.encrypt(n_hash)) == n_hash);
}
