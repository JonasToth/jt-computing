#include "jt-computing/math/BigUInt.hpp"
#include "jt-computing/math/GenericPower.hpp"
#include "jt-computing/math/ModularArithmetic.hpp"
#include "jt-computing/math/NaturalN.hpp"

#include <chrono>

using namespace jt;
using namespace jt::math;
using namespace std;
namespace {
template <typename Int = BigUInt>
Int fromHexString(const std::string &numberHex) {
  Int result{0U};
  std::istringstream{numberHex} >> std::hex >> result;
  return result;
}
template <typename Int = BigUInt> std::string toHexString(const Int &n) {
  std::ostringstream oss;
  oss << std::hex << n;
  return oss.str();
}
template <typename Int = BigUInt> std::string toString(const Int &n) {
  std::ostringstream oss;
  oss << std::dec << n;
  return oss.str();
}
template <typename Int> auto measureSignature() {
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
  auto n_modulus          = fromHexString<Int>(modulus);
  auto n_public_exponent  = fromHexString<Int>(public_exponent);
  auto n_private_exponent = fromHexString<Int>(private_exponent);
  auto n_prime1           = fromHexString<Int>(prime1);
  auto n_prime2           = fromHexString<Int>(prime2);
  auto n_exponent1        = fromHexString<Int>(exponent1);
  auto n_exponent2        = fromHexString<Int>(exponent2);
  auto n_coefficient      = fromHexString<Int>(coefficient);
  auto n_hash             = fromHexString<Int>(hash_document);
  const auto before       = std::chrono::steady_clock::now();
  const auto signature =
      power_monoid(n_hash, n_private_exponent, multiplies_mod{n_modulus});
  const auto after = std::chrono::steady_clock::now();
  return after - before;
}
} // namespace

int main() {
#if 0
  const auto dBigUInt = measureSignature<BigUInt>();
  std::cout << "Naive Bit-BigUInt takes "
            << chrono::duration_cast<chrono::milliseconds>(dBigUInt)
            << " for signature calculation" << std::endl;
#endif
  const auto dNaturalN = measureSignature<NaturalN>();
  std::cout << "Naive u32-NaturalN takes "
            << chrono::duration_cast<chrono::milliseconds>(dNaturalN)
            << " for signature calculation" << std::endl;
  return EXIT_SUCCESS;
}
