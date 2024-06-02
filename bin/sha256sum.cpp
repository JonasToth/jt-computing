#include <algorithm>
#include <cassert>
#include <cstring>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <syncstream>
#include <vector>

#include "jt-computing/crypto/Sha256.hpp"

namespace {
struct HashResult {
  std::string digest;
  std::filesystem::path path_arg;
};

HashResult computeHashForFile(std::filesystem::path arg) {
  if (!is_regular_file(arg)) {
    return {"", std::move(arg)};
  }

  using namespace jt::crypto;
  auto hasher = Sha256Sum{};
  auto read   = std::fstream{arg, std::ios_base::in | std::ios_base::binary};
  hasher.process(std::istreambuf_iterator<char>(read),
                 std::istreambuf_iterator<char>());
  return {hasher.digest(), std::move(arg)};
}

std::vector<std::filesystem::path> filesFromArgv(std::span<char const *> args) {
  assert(args.size() >= 2 && "At least one file argument required");
  auto files = std::vector<std::filesystem::path>{};
  files.reserve(args.size() - 1);
  for (auto const *a : args.subspan(1)) {
    files.emplace_back(a);
  }
  return files;
}

std::vector<HashResult>
computeHashes(std::vector<std::filesystem::path> const &files) {
  auto result = std::vector<HashResult>{files.size()};
  std::transform(std::execution::par, files.begin(), files.end(),
                 result.begin(), computeHashForFile);

  return result;
}

void hashPrinter(HashResult const &r) {
  if (r.digest.empty()) {
    std::cerr << "File: '" << r.path_arg << "' could not be hashed!\n";
    return;
  }
  std::cout << r.digest << " " << r.path_arg.string() << "\n";
}

void outputHashes(std::vector<HashResult> const &hashes) {
  std::ranges::for_each(hashes, hashPrinter);
  std::cout << std::flush;
}

std::vector<HashResult> readHashes(std::filesystem::path const &inFile) {
  auto in     = std::fstream{inFile, std::ios_base::in};
  auto result = std::vector<HashResult>{};
  for (std::string line; std::getline(in, line);) {
    if (line.size() < 65) {
      std::cerr
          << "Line '" << line << "'"
          << " is too short to contain a valid sha256 hash and file path!\n";
      continue;
    }
    auto digest = std::string{};
    auto path   = std::filesystem::path{};
    auto ss     = std::istringstream{line};
    ss >> digest >> path;
    result.emplace_back(digest, path);
  }
  return result;
}

using HashComparison = std::pair<HashResult, HashResult const *>;
HashComparison verifyHash(HashResult const &expectedResult) {
  auto const actual = computeHashForFile(expectedResult.path_arg);
  return std::make_pair(actual, &expectedResult);
}

bool filesMatchExpectedHashes(std::vector<HashResult> const &expectedHashes) {
  auto comparison = std::vector<HashComparison>(expectedHashes.size());
  std::transform(std::execution::par, expectedHashes.begin(),
                 expectedHashes.end(), comparison.begin(), verifyHash);
  return std::all_of(
      comparison.begin(), comparison.end(), [&](HashComparison const &hc) {
        bool const matches = hc.first.digest == hc.second->digest;
        if (!matches) {
          std::cerr << "File: '" << hc.second->path_arg
                    << "' has mismatching hashes. Actual: " << hc.first.digest
                    << " - Expected: " << hc.second->digest << std::endl;
        } else {
          std::cerr << hc.second->path_arg.string() << ": OK" << std::endl;
        }
        return matches;
      });
}

} // namespace

int main(int argc, char const **argv) {
  auto const args = std::span{argv, static_cast<std::size_t>(argc)};
  if (args.size() < 2) {
    std::cerr << "Error: " << args[0]
              << " - At least one file to hash must be provided!\n";
    return EXIT_FAILURE;
  }
  assert(argc >= 2 && "At least one argument is guaranteed from here on");
  if (strcmp(args[1], "-c") == 0 || strcmp(args[1], "--check") == 0) {
    if (args.size() != 3) {
      std::cerr << "Error: " << args[0]
                << " - Checking requires the hash-file as third argument!\n";
      return EXIT_FAILURE;
    }
    auto const expectedHashes = readHashes(args[2]);
    return filesMatchExpectedHashes(expectedHashes) ? EXIT_SUCCESS
                                                    : EXIT_FAILURE;
  }
  auto const files  = filesFromArgv(args);
  auto const result = computeHashes(files);
  outputHashes(result);

  return EXIT_SUCCESS;
}
