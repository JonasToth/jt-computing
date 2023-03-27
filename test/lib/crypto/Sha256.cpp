#include "jt-computing/crypto/Sha256.hpp"

#include <cstdio>
#include <forward_list>
#include <iterator>
#include <ranges>
#include <sstream>

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace jt;
using namespace jt::crypto;

TEST_CASE("Empty String", "") {
  Sha256Sum s;
  REQUIRE(s.digest() ==
          "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}
TEST_CASE("ShortString", "") {
  Sha256Sum s;
  s.process("Hello");
  REQUIRE(s.digest() ==
          "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969");
}
TEST_CASE("MiddleLong String", "") {
  auto data = std::string{"oalksdjf oiajsdoifjqlwejroi jasdflk alskjdj"};
  Sha256Sum s;
  s.process(data);
  REQUIRE(s.digest() ==
          "4ca4b1ca6bed53768616a17388163e2654abab50c1d59fce26b5b22fe815b1a0");
}
TEST_CASE("MiddleLong String > 56 && < 64", "") {
  auto data = std::string{"oalksdjf oiajsdoifjqlwejroi "
                          "jasdflk alskjdja ahiuh11l2kj232"};
  Sha256Sum s;
  s.process(data);
  REQUIRE(s.digest() ==
          "77873066c366025abc49fb5d7cb5a24a2a9cbe7117c82c30996d62525a528ed8");
}
TEST_CASE("MiddleLong String == 64", "") {
  auto data = std::string{"oalksdjf oiajsdoifjqlwejroi "
                          "jasdflk alskjdja ahiuh11l2kj232"
                          "hquwe"};
  Sha256Sum s;
  s.process(data);
  REQUIRE(s.digest() ==
          "207d2bb4e16a569f47fdf7afff9016ee88289b8af5808aac52194f03f08cc9e6");
}
TEST_CASE("MiddleLong String == 70", "") {
  auto data = std::string{"oalksdjf oiajsdoifjqlwejroi "
                          "jasdflk alskjdja ahiuh11l2kj232"
                          "hquwe asu13"};
  Sha256Sum s;
  s.process(data);
  REQUIRE(s.digest() ==
          "e145971a579a6d748dae0ac60b4ed5ed866ec53a91adecb96ef025baf4d7964a");
}
TEST_CASE("Lorem Ipseum Processed Differently", "") {
  const auto expectedHash = std::string{
      "bac1ff482adf0f85340f18db432d0dc281d6207eb06a122b4a1a96af8dbd8c92"};
  const auto lorem = std::string{
      R"(Quia ut laborum rem ut consectetur consectetur consectetur. Ex ea ullam
eaque quaerat sint quaerat sit. Saepe quod rerum qui veniam quia 
nostrum quasi ex. Nihil ab voluptates non quasi eos dolores ducimus 
nemo.

Numquam facere aut facere molestias eos voluptatem at. A quos 
consequatur quia minus consequatur at. Laboriosam ipsum odit quod 
aperiam eaque voluptas ut. Qui alias magnam a fugiat officiis.
Laboriosam ut enim quibusdam magnam rerum. Repellendus nesciunt officia 
corporis sit. Voluptatum ut reiciendis eius saepe natus sed est ut. 
Veniam quia maxime harum in. Maiores sit molestias quia enim quae eius 
ducimus dolorem. Voluptatum suscipit autem eaque quod autem quibusdam 
suscipit assumenda.

Quo eum esse quia neque aut aut. Sed corporis eaque eos. Voluptas 
aperiam illum eligendi ipsam ipsam. Enim consequatur aut voluptates 
numquam veniam. Alias repellendus molestias voluptas fugit alias optio.
Beatae incidunt eum non repellat. Quia et in voluptatum harum suscipit 
perspiciatis eius sunt. A error maiores voluptate ipsam.

Est sunt natus impedit. Accusamus iure excepturi animi ut. In dolorem 
et deserunt necessitatibus odio vel consequuntur. Aperiam dolor 
voluptas enim ducimus aliquam omnis.

Temporibus tenetur ducimus iusto adipisci a ipsa velit. Aperiam quo et 
ab recusandae architecto. Quidem ut aut et veniam earum.
Aut vitae architecto consequatur soluta est repellendus ab. Modi odio 
est amet. Provident nostrum quidem repellendus est architecto. Eius 
consequuntur et sequi. Autem atque id facilis id repellendus debitis 
ipsam quia.

Ducimus perspiciatis autem atque sapiente dolores corrupti sed at. Est 
commodi deleniti eum. Id assumenda dolor sint aut ullam voluptas optio. 
Maxime ratione dolorem modi accusamus voluptates. Maxime adipisci est 
quaerat qui.

Alias laborum qui dolores et omnis autem consectetur. Recusandae ad 
quasi repellendus. Eligendi qui veniam eveniet est dolores ut. Est 
ducimus non amet assumenda.)"};
  Sha256Sum s;
  s.process(lorem);
  REQUIRE(s.digest() == expectedHash);

  // 367 is randomly selected to be not evenly divisible by the hashing chunks.
  // 367 is prime.
  for (usize chunkSize = 1; chunkSize < 367; ++chunkSize) {
    const auto numberChunks = lorem.size() / chunkSize;
    const auto remainder    = lorem.size() % chunkSize;
    const auto sv           = std::string_view{lorem};
    Sha256Sum consecutiveHashed;
    for (usize i = 0; i < numberChunks; ++i) {
      consecutiveHashed.process(sv.substr(i * chunkSize, chunkSize));
    }
    consecutiveHashed.process(sv.substr(numberChunks * chunkSize, remainder));
    REQUIRE(consecutiveHashed.digest() == expectedHash);
  }
}

TEST_CASE("Sha256 resets", "") {
  const auto *s1 = "I am the first string";
  const auto *s2 = "Completely different content";
  Sha256Sum s;

  s.process(s1);
  REQUIRE(s.digest() ==
          "fbda3f5df2fbc9a4811ad7cf6fec0cda2ee0b8a11311561fb117f983414158e2");
  s.reset();

  s.process(s2);
  REQUIRE(s.digest() ==
          "035bb26c5292066fbdb56e6c05b026bb7b4392e835d976880148c857c614c7a3");

  s.reset();
  REQUIRE(s.digest() ==
          "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST_CASE("Sha256 throws when digested, but not reset", "") {
  const auto *s1 = "I am the first string";
  const auto *s2 = "Completely different content";
  Sha256Sum s;

  s.process(s1);
  REQUIRE(s.digest() ==
          "fbda3f5df2fbc9a4811ad7cf6fec0cda2ee0b8a11311561fb117f983414158e2");
  s.reset();

  s.process(s2);
  REQUIRE(s.digest() ==
          "035bb26c5292066fbdb56e6c05b026bb7b4392e835d976880148c857c614c7a3");

  REQUIRE_THROWS_AS(s.process("More Content"), std::runtime_error);
}

TEST_CASE("Hash single-linked list of characters", "") {
  const auto expectedHash = std::string_view{
      "6da1d8d43d91f1af32775e7ec6b1425235b8f31072271b792635cc5451bf2793"};
  const auto s1 = std::string_view{"I am the worst string"};
  auto s        = Sha256Sum{};
  s.process(s1);
  REQUIRE(s.digest() == expectedHash);
  s.reset();

  const auto charList = std::forward_list<char>{std::begin(s1), std::end(s1)};
  s.process(charList);
  REQUIRE(s.digest() == expectedHash);
}

TEST_CASE("Hash istream-view", "") {
  auto iss = std::istringstream{"Stream Class Iterator hashing works"};
  auto s   = Sha256Sum{};

  s.process(iss.view());
  REQUIRE(s.digest() ==
          "fa14ee12d646c67c045bda06c5cfee49be15ea5ffac34b1da25a1b86fb156832");
}
TEST_CASE("Hash istream range", "") {
  auto iss = std::istringstream{"Temporary File Content\r\n"};

  auto s   = Sha256Sum{};
  s.process(std::ranges::subrange{std::istreambuf_iterator<char>{iss},
                                  std::istreambuf_iterator<char>{}});

  REQUIRE(s.digest() ==
          "c04084102785173be85abc3cdd55478facd9833d5fe4062e706992da30ff852d");
}
TEST_CASE("Hash istream iterator-interface", "") {
  auto iss = std::istringstream{"Temporary File Content\r\n"};

  auto s   = Sha256Sum{};
  s.process(std::istreambuf_iterator<char>{iss},
            std::istreambuf_iterator<char>{});

  REQUIRE(s.digest() ==
          "c04084102785173be85abc3cdd55478facd9833d5fe4062e706992da30ff852d");
}

TEST_CASE("Benchmark for Sha256", "[.]") {
  auto generateToHash = []() -> std::string {
    auto r           = std::string{};
    const auto mb256 = 1U << 28U;
    r.reserve(mb256);
    for (unsigned i = 0; i < mb256; ++i) {
      const auto ch = 'A' + (i % 26);
      r += static_cast<char>(ch);
    }
    return r;
  };
  const auto refStr = generateToHash();
  BENCHMARK("Hash 256MB string") {
    auto shaHash = Sha256Sum{};
    shaHash.process(refStr);
    return shaHash.digest();
  };
}
