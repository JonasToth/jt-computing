#include <algorithm>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "jt-computing/crypto/Sha256.hpp"

namespace {
struct HashResult {
  std::string digest;
  char const *path_arg;
};
HashResult computeHashForFile(char const *arg) {
  std::filesystem::path p{arg};
  if (!is_regular_file(p)) {
    return {"", arg};
  }

  using namespace jt::crypto;
  auto hasher = Sha256Sum{};
  std::fstream read{p, std::ios_base::in | std::ios_base::binary};
  hasher.process(std::istreambuf_iterator<char>(read),
                 std::istreambuf_iterator<char>());
  return {hasher.digest(), arg};
}
} // namespace

int main(int argc, char const **argv) {
  if (argc < 2) {
    std::cerr << "Error: " << argv[0]
              << " - At least one file to hash must be provided!\n";
    return EXIT_FAILURE;
  }
  std::vector<char const *> files;
  for (int i = 1; i < argc; ++i) {
    files.emplace_back(argv[i]);
  }

  std::vector<HashResult> result(files.size());
  std::transform(std::execution::par, files.begin(), files.end(),
                 result.begin(), computeHashForFile);

  std::ranges::for_each(result, [](auto &&r) {
    if (r.digest.empty()) {
      std::cerr << "File: '" << r.path_arg << "' could not be hashed!\n";
      return;
    }
    std::cout << r.digest << "  " << r.path_arg << "\n";
  });
  std::cout << std::flush;

  return EXIT_SUCCESS;
}
