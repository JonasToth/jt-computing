#include "jt-computing/core/Contracts.hpp"

import std;
import jt.Math;

using namespace std;
using namespace jt;
using namespace jt::math;

namespace {

template <i32 GridSize> struct GridNode {
  i32 row    = 0;
  i32 column = 0;
};
template <i32 GridSize>
bool operator<(const GridNode<GridSize> &n1, const GridNode<GridSize> &n2) {
  return (n1.row * GridSize + n1.column) < (n2.row * GridSize + n2.column);
}

template <i32 N>
ostream &operator<<(
    ostream &os,
    const FixedSquareMatrix<bool, N, logical_or<bool>, logical_and<bool>> &m) {
  for (i32 i = 0; i < N; ++i) {
    for (i32 j = 0; j < N; ++j) {
      os << (m(i, j) ? "x" : " ");
    }
    os << "\n";
  }
  return os;
}
} // namespace

int main(int argc, char **argv) {
  if (argc != 3 && argc != 4) {
    CONTRACT_ASSERT(argc >= 1);
    cerr << "Usage: " << argv[0]
         << " <probability-threshold> <output-file> <seed>\n\n"
         << "Computes clusters in percolation simulation.\n";
    return EXIT_FAILURE;
  }
  const auto probability = float(atof(argv[1]));
  if (probability > 1.0F || probability < 0.0F) {
    cerr << "Probability must be between [0.0, 1.0]! Got: " << probability
         << endl;
    return EXIT_FAILURE;
  }
  const auto threshold = 1.0F - probability;
  auto rd              = random_device{};
  const auto seed      = argc == 4 ? unsigned(atoi(argv[3])) : rd();
  auto gen             = mt19937{seed};
  auto dist            = uniform_real_distribution<float>{0.0, 1.0};

  cout << "Ranomd Device Seeded with: " << seed << "\n";

  constexpr auto gridWidth  = 25;
  constexpr auto matrixSize = gridWidth * gridWidth;
  auto probabilityGraph     = FixedSquareMatrix<float, matrixSize>(1);
  const auto edgeIdx        = [](i32 r, i32 c) { return r * gridWidth + c; };

  // Construct the adjacency matrix with probabilities first.
  for (i32 row = 0; row < gridWidth; ++row) {
    for (i32 col = 0; col < gridWidth; ++col) {
      const auto centerIdx = edgeIdx(row, col);

      // "Upper" neighbour exists.
      if (row > 0) {
        const auto upperIdx                   = edgeIdx(row - 1, col);
        const auto upperP                     = dist(gen);
        // The connection relationship is symmetrical.
        probabilityGraph(centerIdx, upperIdx) = upperP;
        probabilityGraph(upperIdx, centerIdx) = upperP;
      }
      // "Left" neighbour exists.
      if (col > 0) {
        const auto leftIdx                   = edgeIdx(row, col - 1);
        const auto leftP                     = dist(gen);
        // The connection relationship is symmetrical.
        probabilityGraph(centerIdx, leftIdx) = leftP;
        probabilityGraph(leftIdx, centerIdx) = leftP;
      }
    }
  }
  auto adjancyMatrix =
      FixedSquareMatrix<bool, matrixSize, logical_or<bool>, logical_and<bool>>(
          0U);
  for (i32 row = 0; row < gridWidth; ++row) {
    for (i32 col = 0; col < gridWidth; ++col) {
      const auto idx = edgeIdx(row, col);
      for (i32 i = 0; i < matrixSize; ++i) {
        adjancyMatrix(i, idx) = probabilityGraph(i, idx) >= threshold;
      }
    }
  }

  const auto startPower   = chrono::steady_clock::now();
  const auto clusterGraph = power_monoid(adjancyMatrix, matrixSize - 1);
  const auto endPower     = chrono::steady_clock::now();
  cout << "ClusterGraph:\n" << clusterGraph << "\n";

  const auto maxColors       = 16;
  auto clusterColor          = map<GridNode<gridWidth>, int>{};
  auto nodesWithoutComponent = set<GridNode<gridWidth>>{};
  for (i32 row = 0; row < gridWidth; ++row) {
    for (i32 col = 0; col < gridWidth; ++col) {
      nodesWithoutComponent.insert({row, col});
    }
  }

  const auto startCluster = chrono::steady_clock::now();
  auto clusters = map<GridNode<gridWidth>, set<GridNode<gridWidth>>>{};

  for (i32 color = 0; !nodesWithoutComponent.empty();
       color     = (color + 1) % maxColors) {
    // Extract the next node that forms a component.
    const auto nodeIt = nodesWithoutComponent.begin();
    const auto newC   = *nodeIt;
    nodesWithoutComponent.erase(nodeIt);

    auto newCluster = set<GridNode<gridWidth>>{newC};

    for (auto it = nodesWithoutComponent.begin();
         it != nodesWithoutComponent.end();) {
      const auto node           = *it;
      const auto idxThisCluster = edgeIdx(newC.row, newC.column);
      const auto idxOther       = edgeIdx(node.row, node.column);
      if (clusterGraph(idxThisCluster, idxOther)) {
        newCluster.insert(*it);
        it = nodesWithoutComponent.erase(it);
      } else {
        ++it;
      }
    }

    auto [component, inserted] = clusters.insert({newC, move(newCluster)});
    if (!inserted) {
      throw logic_error{"Node already exists in different cluster."};
    }
    clusterColor[newC] = color;
  }
  const auto endCluster = chrono::steady_clock::now();

  auto clusteredNodes   = 0;
  for (const auto &[startNode, elements] : clusters) {
    cout << "Cluster at (" << startNode.row << ", " << startNode.column
         << ") with " << elements.size() << " elements.\n";
    clusteredNodes += int(elements.size());
  }
  cout << "Cluster Graph Computation took "
       << chrono::duration_cast<chrono::milliseconds>(endPower - startPower)
              .count()
       << " ms\n";
  cout << "Cluster Computation took "
       << chrono::duration_cast<chrono::microseconds>(endCluster - startCluster)
              .count()
       << " us\n";
  if (clusteredNodes != matrixSize) {
    throw logic_error{"All nodes must be part of a cluster and always sum "
                      "up to matrix size. Graph Invariant Violated"};
  }

  ofstream graphOutput{argv[2]};
  for (const auto &[initialNode, elements] : clusters) {
    for (const auto &node : elements) {
      graphOutput << node.column << " " << node.row << " "
                  << clusterColor.at(initialNode) << "\n";
    }
  }
  return 0;
}
