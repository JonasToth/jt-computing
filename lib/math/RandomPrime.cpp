#include "jt-computing/math/RandomPrime.hpp"
#include "jt-computing/math/GenericPower.hpp"
#include "jt-computing/math/ModularArithmetic.hpp"

#include <algorithm>
#include <cassert>
#include <climits>
#include <execution>
#include <iostream>
#include <random>

namespace jt::math {
NaturalN generateRandomNumber(i32 bits) {
  [[maybe_unused]] auto const originalBits = bits;
  assert(originalBits > 0 && "Positive number of bits required");

  std::mt19937 twister{std::random_device{}()};
  constexpr i32 bitsPerIteration =
      sizeof(std::random_device::result_type) * CHAR_BIT;

  --bits;
  auto result = 1_U;

  for (; bits >= bitsPerIteration; bits -= bitsPerIteration) {
    result <<= bitsPerIteration;
    result += NaturalN{twister()};
  }
  if (bits > 0) {
    assert(bits < bitsPerIteration &&
           "Number of bits must have been reduced already");

    result <<= bits;

    auto finalBits     = twister();
    auto const toShift = static_cast<u32>(bitsPerIteration - bits);

    finalBits >>= toShift;
    result += NaturalN{finalBits};
  }

  assert(result.binaryDigits() == static_cast<u64>(originalBits) &&
         "Generated number does not match the expected bits");

  return result;
}

NaturalN randomNumberSmallerThan(NaturalN const &threshold) {
  auto bits = threshold.binaryDigits();
  assert(bits > 0 && "Positive number of bits required");

  std::mt19937 twister{std::random_device{}()};
  constexpr i32 bitsPerIteration =
      sizeof(std::random_device::result_type) * CHAR_BIT;

  auto result = 0_U;

  for (; bits >= bitsPerIteration; bits -= bitsPerIteration) {
    result <<= bitsPerIteration;
    result += NaturalN{twister()};
  }
  if (bits > 0) {
    assert(bits < bitsPerIteration &&
           "Number of bits must have been reduced already");

    result <<= static_cast<i32>(bits);

    auto finalBits     = twister();
    auto const toShift = static_cast<u32>(bitsPerIteration - bits);

    finalBits >>= toShift;
    result += NaturalN{finalBits};
  }

  return result;
}

namespace {
bool isDivisibleBySmallPrime(NaturalN const &number) {
  if (number == 0_U || number == 1_U) {
    return true;
  }
  static std::vector<NaturalN> const firstPrimes{
      2_U,    3_U,    5_U,    7_U,    11_U,   13_U,   17_U,   19_U,   23_U,
      29_U,   31_U,   37_U,   41_U,   43_U,   47_U,   53_U,   59_U,   61_U,
      67_U,   71_U,   73_U,   79_U,   83_U,   89_U,   97_U,   101_U,  103_U,
      107_U,  109_U,  113_U,  127_U,  131_U,  137_U,  139_U,  149_U,  151_U,
      157_U,  163_U,  167_U,  173_U,  179_U,  181_U,  191_U,  193_U,  197_U,
      199_U,  211_U,  223_U,  227_U,  229_U,  233_U,  239_U,  241_U,  251_U,
      257_U,  263_U,  269_U,  271_U,  277_U,  281_U,  283_U,  293_U,  307_U,
      311_U,  313_U,  317_U,  331_U,  337_U,  347_U,  349_U,  353_U,  359_U,
      367_U,  373_U,  379_U,  383_U,  389_U,  397_U,  401_U,  409_U,  419_U,
      421_U,  431_U,  433_U,  439_U,  443_U,  449_U,  457_U,  461_U,  463_U,
      467_U,  479_U,  487_U,  491_U,  499_U,  503_U,  509_U,  521_U,  523_U,
      541_U,  547_U,  557_U,  563_U,  569_U,  571_U,  577_U,  587_U,  593_U,
      599_U,  601_U,  607_U,  613_U,  617_U,  619_U,  631_U,  641_U,  643_U,
      647_U,  653_U,  659_U,  661_U,  673_U,  677_U,  683_U,  691_U,  701_U,
      709_U,  719_U,  727_U,  733_U,  739_U,  743_U,  751_U,  757_U,  761_U,
      769_U,  773_U,  787_U,  797_U,  809_U,  811_U,  821_U,  823_U,  827_U,
      829_U,  839_U,  853_U,  857_U,  859_U,  863_U,  877_U,  881_U,  883_U,
      887_U,  907_U,  911_U,  919_U,  929_U,  937_U,  941_U,  947_U,  953_U,
      967_U,  971_U,  977_U,  983_U,  991_U,  997_U,  1009_U, 1013_U, 1019_U,
      1021_U, 1031_U, 1033_U, 1039_U, 1049_U, 1051_U, 1061_U, 1063_U, 1069_U,
      1087_U, 1091_U, 1093_U, 1097_U, 1103_U, 1109_U, 1117_U, 1123_U, 1129_U,
      1151_U, 1153_U, 1163_U, 1171_U, 1181_U, 1187_U, 1193_U, 1201_U, 1213_U,
      1217_U, 1223_U, 1229_U, 1231_U, 1237_U, 1249_U, 1259_U, 1277_U, 1279_U,
      1283_U, 1289_U, 1291_U, 1297_U, 1301_U, 1303_U, 1307_U, 1319_U, 1321_U,
      1327_U, 1361_U, 1367_U, 1373_U, 1381_U, 1399_U, 1409_U, 1423_U, 1427_U,
      1429_U, 1433_U, 1439_U, 1447_U, 1451_U, 1453_U, 1459_U, 1471_U, 1481_U,
      1483_U, 1487_U, 1489_U, 1493_U, 1499_U, 1511_U, 1523_U, 1531_U, 1543_U,
      1549_U, 1553_U, 1559_U, 1567_U, 1571_U, 1579_U, 1583_U, 1597_U, 1601_U,
      1607_U, 1609_U, 1613_U, 1619_U, 1621_U, 1627_U, 1637_U, 1657_U, 1663_U,
      1667_U, 1669_U, 1693_U, 1697_U, 1699_U, 1709_U, 1721_U, 1723_U, 1733_U,
      1741_U, 1747_U, 1753_U, 1759_U, 1777_U, 1783_U, 1787_U, 1789_U, 1801_U,
      1811_U, 1823_U, 1831_U, 1847_U, 1861_U, 1867_U, 1871_U, 1873_U, 1877_U,
      1879_U, 1889_U, 1901_U, 1907_U, 1913_U, 1931_U, 1933_U, 1949_U, 1951_U,
      1973_U, 1979_U, 1987_U, 1993_U, 1997_U, 1999_U, 2003_U, 2011_U, 2017_U,
      2027_U, 2029_U, 2039_U, 2053_U, 2063_U, 2069_U, 2081_U, 2083_U, 2087_U,
      2089_U, 2099_U, 2111_U, 2113_U, 2129_U, 2131_U, 2137_U, 2141_U, 2143_U,
      2153_U, 2161_U, 2179_U, 2203_U, 2207_U, 2213_U, 2221_U, 2237_U, 2239_U,
      2243_U, 2251_U, 2267_U, 2269_U, 2273_U, 2281_U, 2287_U, 2293_U, 2297_U,
      2309_U, 2311_U, 2333_U, 2339_U, 2341_U, 2347_U, 2351_U, 2357_U, 2371_U,
      2377_U, 2381_U, 2383_U, 2389_U, 2393_U, 2399_U, 2411_U, 2417_U, 2423_U,
      2437_U, 2441_U, 2447_U, 2459_U, 2467_U, 2473_U, 2477_U, 2503_U, 2521_U,
      2531_U, 2539_U, 2543_U, 2549_U, 2551_U, 2557_U, 2579_U, 2591_U, 2593_U,
      2609_U, 2617_U, 2621_U, 2633_U, 2647_U, 2657_U, 2659_U, 2663_U, 2671_U,
      2677_U, 2683_U, 2687_U, 2689_U, 2693_U, 2699_U, 2707_U, 2711_U, 2713_U,
      2719_U, 2729_U, 2731_U, 2741_U, 2749_U, 2753_U, 2767_U, 2777_U, 2789_U,
      2791_U, 2797_U, 2801_U, 2803_U, 2819_U, 2833_U, 2837_U, 2843_U, 2851_U,
      2857_U, 2861_U, 2879_U, 2887_U, 2897_U, 2903_U, 2909_U, 2917_U, 2927_U,
      2939_U, 2953_U, 2957_U, 2963_U, 2969_U, 2971_U, 2999_U, 3001_U, 3011_U,
      3019_U, 3023_U, 3037_U, 3041_U, 3049_U, 3061_U, 3067_U, 3079_U, 3083_U,
      3089_U, 3109_U, 3119_U, 3121_U, 3137_U, 3163_U, 3167_U, 3169_U, 3181_U,
      3187_U, 3191_U, 3203_U, 3209_U, 3217_U, 3221_U, 3229_U, 3251_U, 3253_U,
      3257_U, 3259_U, 3271_U, 3299_U, 3301_U, 3307_U, 3313_U, 3319_U, 3323_U,
      3329_U, 3331_U, 3343_U, 3347_U, 3359_U, 3361_U, 3371_U, 3373_U, 3389_U,
      3391_U, 3407_U, 3413_U, 3433_U, 3449_U, 3457_U, 3461_U, 3463_U, 3467_U,
      3469_U, 3491_U, 3499_U, 3511_U, 3517_U, 3527_U, 3529_U, 3533_U, 3539_U,
      3541_U, 3547_U, 3557_U, 3559_U, 3571_U, 3581_U, 3583_U, 3593_U, 3607_U,
      3613_U, 3617_U, 3623_U, 3631_U, 3637_U, 3643_U, 3659_U, 3671_U, 3673_U,
      3677_U, 3691_U, 3697_U, 3701_U, 3709_U, 3719_U, 3727_U, 3733_U, 3739_U,
      3761_U, 3767_U, 3769_U, 3779_U, 3793_U, 3797_U, 3803_U, 3821_U, 3823_U,
      3833_U, 3847_U, 3851_U, 3853_U, 3863_U, 3877_U, 3881_U, 3889_U, 3907_U,
      3911_U, 3917_U, 3919_U, 3923_U, 3929_U, 3931_U, 3943_U, 3947_U, 3967_U,
      3989_U, 4001_U, 4003_U, 4007_U, 4013_U, 4019_U, 4021_U, 4027_U, 4049_U,
      4051_U, 4057_U, 4073_U, 4079_U, 4091_U, 4093_U, 4099_U, 4111_U, 4127_U,
      4129_U, 4133_U, 4139_U, 4153_U, 4157_U, 4159_U, 4177_U, 4201_U, 4211_U,
      4217_U, 4219_U, 4229_U, 4231_U, 4241_U, 4243_U, 4253_U, 4259_U, 4261_U,
      4271_U, 4273_U, 4283_U, 4289_U, 4297_U, 4327_U, 4337_U, 4339_U, 4349_U,
      4357_U, 4363_U, 4373_U, 4391_U, 4397_U, 4409_U, 4421_U, 4423_U, 4441_U,
      4447_U, 4451_U, 4457_U, 4463_U, 4481_U, 4483_U, 4493_U, 4507_U, 4513_U,
      4517_U, 4519_U, 4523_U, 4547_U, 4549_U, 4561_U, 4567_U, 4583_U, 4591_U,
      4597_U, 4603_U, 4621_U, 4637_U, 4639_U, 4643_U, 4649_U, 4651_U, 4657_U,
      4663_U, 4673_U, 4679_U, 4691_U, 4703_U, 4721_U, 4723_U, 4729_U, 4733_U,
      4751_U, 4759_U, 4783_U, 4787_U, 4789_U, 4793_U, 4799_U, 4801_U, 4813_U,
      4817_U, 4831_U, 4861_U, 4871_U, 4877_U, 4889_U, 4903_U, 4909_U, 4919_U,
      4931_U, 4933_U, 4937_U, 4943_U, 4951_U, 4957_U, 4967_U, 4969_U, 4973_U,
      4987_U, 4993_U, 4999_U, 5003_U, 5009_U, 5011_U, 5021_U, 5023_U, 5039_U,
      5051_U, 5059_U, 5077_U, 5081_U, 5087_U, 5099_U, 5101_U, 5107_U, 5113_U,
      5119_U, 5147_U, 5153_U, 5167_U, 5171_U, 5179_U, 5189_U, 5197_U, 5209_U,
      5227_U, 5231_U, 5233_U, 5237_U, 5261_U, 5273_U, 5279_U, 5281_U, 5297_U,
      5303_U, 5309_U, 5323_U, 5333_U, 5347_U, 5351_U, 5381_U, 5387_U, 5393_U,
      5399_U, 5407_U, 5413_U, 5417_U, 5419_U, 5431_U, 5437_U, 5441_U, 5443_U,
      5449_U, 5471_U, 5477_U, 5479_U, 5483_U, 5501_U, 5503_U, 5507_U, 5519_U,
      5521_U, 5527_U, 5531_U, 5557_U, 5563_U, 5569_U, 5573_U, 5581_U, 5591_U,
      5623_U, 5639_U, 5641_U, 5647_U, 5651_U, 5653_U, 5657_U, 5659_U, 5669_U,
      5683_U, 5689_U, 5693_U, 5701_U, 5711_U, 5717_U, 5737_U, 5741_U, 5743_U,
      5749_U, 5779_U, 5783_U, 5791_U, 5801_U, 5807_U, 5813_U, 5821_U, 5827_U,
      5839_U, 5843_U, 5849_U, 5851_U, 5857_U, 5861_U, 5867_U, 5869_U, 5879_U,
      5881_U, 5897_U, 5903_U, 5923_U, 5927_U, 5939_U, 5953_U, 5981_U, 5987_U,
      6007_U, 6011_U, 6029_U, 6037_U, 6043_U, 6047_U, 6053_U, 6067_U, 6073_U,
      6079_U, 6089_U, 6091_U, 6101_U, 6113_U, 6121_U, 6131_U, 6133_U, 6143_U,
      6151_U, 6163_U, 6173_U, 6197_U, 6199_U, 6203_U, 6211_U, 6217_U, 6221_U,
      6229_U, 6247_U, 6257_U, 6263_U, 6269_U, 6271_U, 6277_U, 6287_U, 6299_U,
      6301_U, 6311_U, 6317_U, 6323_U, 6329_U, 6337_U, 6343_U, 6353_U, 6359_U,
      6361_U, 6367_U, 6373_U, 6379_U, 6389_U, 6397_U, 6421_U, 6427_U, 6449_U,
      6451_U, 6469_U, 6473_U, 6481_U, 6491_U, 6521_U, 6529_U, 6547_U, 6551_U,
      6553_U, 6563_U, 6569_U, 6571_U, 6577_U, 6581_U, 6599_U, 6607_U, 6619_U,
      6637_U, 6653_U, 6659_U, 6661_U, 6673_U, 6679_U, 6689_U, 6691_U, 6701_U,
      6703_U, 6709_U, 6719_U, 6733_U, 6737_U, 6761_U, 6763_U, 6779_U, 6781_U,
      6791_U, 6793_U, 6803_U, 6823_U, 6827_U, 6829_U, 6833_U, 6841_U, 6857_U,
      6863_U, 6869_U, 6871_U, 6883_U, 6899_U, 6907_U, 6911_U, 6917_U, 6947_U,
      6949_U, 6959_U, 6961_U, 6967_U, 6971_U, 6977_U, 6983_U, 6991_U, 6997_U,
      7001_U, 7013_U, 7019_U, 7027_U, 7039_U, 7043_U, 7057_U, 7069_U, 7079_U,
      7103_U, 7109_U, 7121_U, 7127_U, 7129_U, 7151_U, 7159_U, 7177_U, 7187_U,
      7193_U, 7207_U, 7211_U, 7213_U, 7219_U, 7229_U, 7237_U, 7243_U, 7247_U,
      7253_U, 7283_U, 7297_U, 7307_U, 7309_U, 7321_U, 7331_U, 7333_U, 7349_U,
      7351_U, 7369_U, 7393_U, 7411_U, 7417_U, 7433_U, 7451_U, 7457_U, 7459_U,
      7477_U, 7481_U, 7487_U, 7489_U, 7499_U, 7507_U, 7517_U, 7523_U, 7529_U,
      7537_U, 7541_U, 7547_U, 7549_U, 7559_U, 7561_U, 7573_U, 7577_U, 7583_U,
      7589_U, 7591_U, 7603_U, 7607_U, 7621_U, 7639_U, 7643_U, 7649_U, 7669_U,
      7673_U, 7681_U, 7687_U, 7691_U, 7699_U, 7703_U, 7717_U, 7723_U, 7727_U,
      7741_U, 7753_U, 7757_U, 7759_U, 7789_U, 7793_U, 7817_U, 7823_U, 7829_U,
      7841_U, 7853_U, 7867_U, 7873_U, 7877_U, 7879_U, 7883_U, 7901_U, 7907_U,
      7919_U, 7927_U, 7933_U, 7937_U, 7949_U, 7951_U, 7963_U, 7993_U,
  };
  return std::any_of(std::execution::par, firstPrimes.begin(),
                     firstPrimes.end(), [&](NaturalN const &divisor) {
                       auto const [q, m] = divmod(number, divisor);
                       return m == 0_U;
                     });
}

bool likelyPrimeByMillerRabin(NaturalN const &number) {
  std::cerr << "Number " << number << " must perform Miller-Rabin-Test\n";

  auto const n_minus_1 = number - 1_U;
  auto d               = n_minus_1;
  auto trailingZeros   = n_minus_1.trailingZeroDigits();
  auto s               = NaturalN{trailingZeros};
  d <<= static_cast<i32>(trailingZeros);

  for (int i = 0; i < 30; ++i) {
    auto const a = randomNumberSmallerThan(number);
    std::cerr << "Check Iteration: " << i << " with number " << a << "\n";
    auto const power = power_monoid(a, d, multiplies_mod{number});
    if (power == 1_U) {
      std::cerr << "First Condition is True: " << number << std::endl;
      continue;
    }
    auto const r = randomNumberSmallerThan(s);
    auto const powerAgain =
        power_monoid(a, power_monoid(2_U, r) * d, multiplies_mod{number});
    if (powerAgain == n_minus_1) {
      std::cerr << "Second Condition is True: " << number << std::endl;
      continue;
    }
    return false;
  }
  return true;
}
} // namespace

NaturalN findNextPrime(NaturalN startingPoint) {
  if (startingPoint.isEven()) {
    startingPoint += 1_U;
  }
  while (isDivisibleBySmallPrime(startingPoint) ||
         !likelyPrimeByMillerRabin(startingPoint)) {
    startingPoint += 2_U;
  }
  std::cerr << "Found Prime: " << startingPoint << "\n";
  return startingPoint;
}
} // namespace jt::math
