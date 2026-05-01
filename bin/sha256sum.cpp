#include "jt-computing/core/Contracts.hpp"

import std;
import jt.Core;
import jt.Crypto;

using namespace std;

namespace {
struct HashResult {
  string digest;
  filesystem::path path_arg;
};

HashResult computeHashForFile(filesystem::path arg) {
  if (!is_regular_file(arg)) {
    return {"", move(arg)};
  }

  using namespace jt::crypto;
  auto hasher = Sha256Sum{};
  ifstream file(arg, ios::binary);
  hasher.process(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
  return {hasher.digest(), move(arg)};
}

vector<filesystem::path> filesFromArgv(span<char const *> args) {
  CONTRACT_ASSERT(args.size() >= 2 && "At least one file argument required");
  auto files = vector<filesystem::path>{};
  files.reserve(args.size() - 1);
  for (auto const *a : args.subspan(1)) {
    files.emplace_back(a);
  }
  return files;
}

vector<HashResult> computeHashes(vector<filesystem::path> const &files) {
  auto result = vector<HashResult>{files.size()};
  transform(execution::par, files.begin(), files.end(), result.begin(),
            computeHashForFile);

  return result;
}

void hashPrinter(HashResult const &r) {
  if (r.digest.empty()) {
    cerr << "File: '" << r.path_arg.string() << "' could not be hashed!\n";
    return;
  }
  cout << r.digest << " " << r.path_arg.string() << "\n";
}

void outputHashes(vector<HashResult> const &hashes) {
  ranges::for_each(hashes, hashPrinter);
  cout << flush;
}

vector<HashResult> readHashes(filesystem::path const &inFile) {
  auto in     = fstream{inFile, ios_base::in};
  auto result = vector<HashResult>{};
  for (string line; getline(in, line);) {
    if (line.size() < 65) {
      cerr << "Line '" << line << "'"
           << " is too short to contain a valid sha256 hash and file path!\n";
      continue;
    }
    auto digest = string{};
    auto path   = string{};
    auto ss     = istringstream{line};
    ss >> digest >> path;
    result.emplace_back(digest, filesystem::path{path});
  }
  return result;
}

using HashComparison = pair<HashResult, HashResult const *>;
HashComparison verifyHash(HashResult const &expectedResult) {
  auto const actual = computeHashForFile(expectedResult.path_arg);
  return make_pair(actual, &expectedResult);
}

bool filesMatchExpectedHashes(vector<HashResult> const &expectedHashes) {
  auto comparison = vector<HashComparison>(expectedHashes.size());
  transform(execution::par, expectedHashes.begin(), expectedHashes.end(),
            comparison.begin(), verifyHash);
  return all_of(
      comparison.begin(), comparison.end(), [&](HashComparison const &hc) {
        bool const matches = hc.first.digest == hc.second->digest;
        if (!matches) {
          cerr << "File: '" << hc.second->path_arg.string()
               << "' has mismatching hashes. Actual: " << hc.first.digest
               << " - Expected: " << hc.second->digest << endl;
        } else {
          cerr << hc.second->path_arg.string() << ": OK" << endl;
        }
        return matches;
      });
  return false;
}

} // namespace

int main(int argc, char const **argv) {
  auto const args = span{argv, static_cast<size_t>(argc)};
  if (args.size() < 2) {
    cerr << "Error: " << args[0]
         << " - At least one file to hash must be provided!\n";
    return jt::EXIT_FAILURE;
  }
  CONTRACT_ASSERT(argc >= 2 &&
                  "At least one argument is guaranteed from here on");
  if (strcmp(args[1], "-c") == 0 || strcmp(args[1], "--check") == 0) {
    if (args.size() != 3) {
      cerr << "Error: " << args[0]
           << " - Checking requires the hash-file as third argument!\n";
      return jt::EXIT_FAILURE;
    }
    auto const expectedHashes = readHashes(args[2]);
    return filesMatchExpectedHashes(expectedHashes) ? jt::EXIT_SUCCESS
                                                    : jt::EXIT_FAILURE;
  }
  auto const files  = filesFromArgv(args);
  auto const result = computeHashes(files);
  outputHashes(result);

  return jt::EXIT_SUCCESS;
}
