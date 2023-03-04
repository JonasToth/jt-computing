#include "jt-computing/math/ModularArithmetic.hpp"
#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/Concepts.hpp"
#include "jt-computing/math/GenericPower.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

using namespace jt;
using namespace jt::math;

TEST_CASE("Modular Addition", "") {
  SECTION("small than mod") {
    REQUIRE(plus_mod{57_N}(20_N, 15_N) == 35_N);
    REQUIRE(plus_mod{17_N}(0_N, 15_N) == 15_N);
    REQUIRE(plus_mod{10_N}(5_N, 3_N) == 8_N);
  }

  SECTION("equal mod") {
    REQUIRE(plus_mod{57_N}(20_N, 37_N) == 0_N);
    REQUIRE(plus_mod{17_N}(0_N, 17_N) == 0_N);
    REQUIRE(plus_mod{10_N}(9_N, 1_N) == 0_N);
  }

  SECTION("bigger than mod") {
    REQUIRE(plus_mod{57_N}(30_N, 37_N) == 10_N);
    REQUIRE(plus_mod{17_N}(1239_N, 17_N) == 15_N);
    REQUIRE(plus_mod{10_N}(9_N, 21_N) == 0_N);
  }
}
TEST_CASE("Modular Multiplication", "") {
  SECTION("small than mod") {
    REQUIRE(multiplies_mod{57_N}(2_N, 21_N) == 42_N);
    REQUIRE(multiplies_mod{17_N}(1_N, 15_N) == 15_N);
    REQUIRE(multiplies_mod{10_N}(3_N, 3_N) == 9_N);
  }

  SECTION("equal mod") {
    REQUIRE(multiplies_mod{64_N}(8_N, 8_N) == 0_N);
    REQUIRE(multiplies_mod{17_N}(1_N, 17_N) == 0_N);
    REQUIRE(multiplies_mod{10_N}(5_N, 2_N) == 0_N);
  }

  SECTION("bigger than mod") {
    REQUIRE(multiplies_mod{57_N}(30_N, 37_N) == 27_N);
    REQUIRE(multiplies_mod{17_N}(1239_N, 31_N) == 6_N);
    REQUIRE(multiplies_mod{10_N}(9_N, 2_N) == 8_N);
  }
}
static BigUInt fromHexString(const std::string &numberHex) {
  BigUInt result = 0_N;
  std::istringstream{numberHex} >> std::hex >> result;
  return result;
}
static std::string toHexString(const BigUInt &n) {
  std::ostringstream oss;
  oss << std::hex << n;
  return oss.str();
}
static std::string toString(const BigUInt &n) {
  std::ostringstream oss;
  oss << std::dec << n;
  return oss.str();
}
TEST_CASE("Example RSA", "") {
  using namespace std::string_literals;
  const auto modulus =
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
  const auto prime1 =
      "F819A9BAF42B4707EDE7307B7539E1ACF8A97AF4F74755309F592A7681BEBC1DE2EF11A0"
      "7EA4D075EEAB391B1C4887C921756E5A3167F89F7DAB980F4DB3E7A1"s;
  const auto prime2 =
      "C966216B413A9446CE67B80DAA94583C8324FE453CE4735620CD30E9A41A1306F181CF3B"
      "28CCE0547ED5D593CD35556754CC30293B658ACE64CAE5FCFC1E532F"s;
  const auto exponent1 =
      "1D6DFDE23D607CD685F3EC9E58737B3FA767833C57B0D07C2A0ACBACAF0B4F0944881351"
      "34749C7DC0C7F2C8327CB00EBDB74E55C8928ABD708CD046D072CCC1"s;
  const auto exponent2 =
      "1E921E2885B23AA7B4D5119F21717B235454DD33ED56501B96C70ED1A8533CE824E8AB68"
      "337D45E00D90AFE6CB9378EF4273EC2B961487C9648B57F5DADF4F89"s;
  const auto coefficient =
      "077C0542F35710264E0D6B0478A2F8B20934C1F7789ABD6EA8AE2FD9BE63FBDCE052BA2A"
      "98568ACF1589E4B0F7F063649E88D1828C37A356CAB4AE0A6D31E166"s;

  const auto hash_document =
      "abe6fe6030068e1f1e7c72c7aad54b77247b48e386a50cdc556a36ec5986d135"s;

  auto n_modulus          = fromHexString(modulus);
  auto n_public_exponent  = fromHexString(public_exponent);
  auto n_private_exponent = fromHexString(private_exponent);
  auto n_prime1           = fromHexString(prime1);
  auto n_prime2           = fromHexString(prime2);
  auto n_exponent1        = fromHexString(exponent1);
  auto n_exponent2        = fromHexString(exponent2);
  auto n_coefficient      = fromHexString(coefficient);
  auto n_hash             = fromHexString(hash_document);

#if 0
  SECTION("Good Values") {
    REQUIRE(n_public_exponent == 65537_N);
    REQUIRE(toString(n_hash) == "7775362756942223229477270152222009939428593292"
                                "5889711304898348330393612505397");
    REQUIRE(toString(n_private_exponent) ==
            "132687760831157997173534550746869794686579580050563210254007193414"
            "522284301208309175504865752353494309240987313726469164842577864253"
            "473676485502686431322369337748341760212714365410175357646166892140"
            "326685847448539711680304877392727651692082178918071388228023684118"
            "649701921862454652633985309992251262516573633");
    REQUIRE(toString(n_modulus) ==
            "137062932959123676582267063634606442341782109508609994647598225783"
            "080572878056410409568325113279075680427560652213596180255150531792"
            "574739314845765003539689698507596892902835891339194293331138132802"
            "041509092383265362469942193975261097283153424310593862248285881327"
            "452080011270392532515677467650035711536445839");
  }

  SECTION("Signing") {
    const auto signature =
        generic_power(n_hash, n_private_exponent, multiplies_mod{n_modulus});
    REQUIRE(toString(signature) ==
            "111356843273362423812252777403757580054118571691274326000973793645"
            "219624068128449318468988265321668145418849363832678480795256997925"
            "993102512061542897135141813272724296255834069914147013797713210478"
            "678409412467010684690667631646142595314849181653509336577768100651"
            "389657739521351496610616116956014980230816311");
    // Python3 naive textbook RSA result.
    REQUIRE(toHexString(signature) ==
            "9e93d10cc246742b8b3d4bc4c0cf5bb827853fc09f58d25e562e2c6bea7150f657"
            "f74fef121e611ea95d3bdc84b83517b64653d811282b497adeaaa17a85c7cc0554"
            "cc177d8e6e7ddea36989d8798ef312d7a840e8d211933a68d28d1e9f31b13c04df"
            "e0c893c6f0a9fa01c67b2ef113e34ab3681271e90df81a1d899cec2237");

    const auto verify_signature =
        generic_power(signature, n_public_exponent, multiplies_mod{n_modulus});

    REQUIRE(verify_signature == n_hash);

    const auto opensslSignature =
        "1812c529070eb143779663ec4633c87dd594495f26fc35c3acacd36816ceb3ff6a87eb"
        "c036ef5b86f4ea5a28551f983f775bb6c525da1d39ffb7d3f283af9709707fc654f727"
        "2962c28a16029ee86747901eac0046fdad38674d7867128c69adc46de369391fb875c0"
        "de43141215b4bb2bdf968bcf555169a08d92982bc77acd"s;
    const auto opensslSignatureInt = fromHexString(opensslSignature);
    REQUIRE(opensslSignatureInt != 0_N);

    // Invert the OpenSSL signature.
    const auto opensslVerify = generic_power(
        opensslSignatureInt, n_public_exponent, multiplies_mod{n_modulus});
    // Convert the result to a hex-string.
    const auto opensslInvertHex = toHexString(opensslVerify);
    const auto signaturePadding =
        "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
        "fffffffff003031300d060960864801650304020105000420"s;
    REQUIRE(opensslInvertHex == signaturePadding + hash_document);
  }

#if 0
  SECTION("Encryption") {
    const auto cipher_hash =
        generic_power(n_hash, n_private_exponent, multiplies_mod{n_modulus});
    const auto decipher_hash = generic_power(cipher_hash, n_public_exponent,
                                               multiplies_mod{n_modulus});
    REQUIRE(decipher_hash == n_hash);
    // REQUIRE(toHexString(cipher_hash) == "");
  }
#endif
#endif
}
