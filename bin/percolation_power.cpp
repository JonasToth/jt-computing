#include "jt-computing/math/FixedSquareMatrix.hpp"
#include "jt-computing/math/GenericPower.hpp"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <set>

using namespace std;
using namespace jt;
using namespace jt::math;

namespace {

template <int GridSize> struct GridNode {
  int row    = 0;
  int column = 0;
};
template <int GridSize>
bool operator<(const GridNode<GridSize> &n1, const GridNode<GridSize> &n2) {
  return (n1.row * GridSize + n1.column) < (n2.row * GridSize + n2.column);
}

template <int N>
std::ostream &operator<<(std::ostream &os,
                         const FixedSquareMatrix<bool, N, std::logical_or<bool>,
                                                 std::logical_and<bool>> &m) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      os << (m(i, j) ? "x" : " ");
    }
    os << "\n";
  }
  return os;
}
} // namespace

int main(int argc, char **argv) {
  if (argc != 3 && argc != 4) {
    assert(argc >= 1);
    cerr << "Usage: " << argv[0]
         << " <probability-threshold> <output-file> <seed>\n\n"
         << "Computes clusters in percolation simulation.\n";
    return EXIT_FAILURE;
  }
  const auto probability = float(std::atof(argv[1]));
  if (probability > 1.0F || probability < 0.0F) {
    std::cerr << "Probability must be between [0.0, 1.0]! Got: " << probability
              << std::endl;
    return EXIT_FAILURE;
  }
  const auto threshold = 1.0F - probability;
  auto rd              = std::random_device{};
  const auto seed      = argc == 4 ? unsigned(std::atoi(argv[3])) : rd();
  auto gen             = std::mt19937{seed};
  auto dist            = std::uniform_real_distribution<float>{0.0, 1.0};

  std::cout << "Ranomd Device Seeded with: " << seed << "\n";

  constexpr auto gridWidth  = 25;
  constexpr auto matrixSize = gridWidth * gridWidth;
  auto probabilityGraph     = FixedSquareMatrix<float, matrixSize>(1);
  const auto edgeIdx        = [](int r, int c) { return r * gridWidth + c; };

  // Construct the adjacency matrix with probabilities first.
  for (int row = 0; row < gridWidth; ++row) {
    for (int col = 0; col < gridWidth; ++col) {
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
      FixedSquareMatrix<bool, matrixSize, std::logical_or<bool>,
                        std::logical_and<bool>>(0U);
  for (int row = 0; row < gridWidth; ++row) {
    for (int col = 0; col < gridWidth; ++col) {
      const auto idx = edgeIdx(row, col);
      for (int i = 0; i < matrixSize; ++i) {
        adjancyMatrix(i, idx) = probabilityGraph(i, idx) >= threshold;
      }
    }
  }

  const auto startPower   = std::chrono::steady_clock::now();
  const auto clusterGraph = power_monoid(adjancyMatrix, matrixSize - 1);
  const auto endPower     = std::chrono::steady_clock::now();
  std::cout << "ClusterGraph:\n" << clusterGraph << "\n";

  const auto maxColors       = 16;
  auto clusterColor          = std::map<GridNode<gridWidth>, int>{};
  auto nodesWithoutComponent = std::set<GridNode<gridWidth>>{};
  for (int row = 0; row < gridWidth; ++row) {
    for (int col = 0; col < gridWidth; ++col) {
      nodesWithoutComponent.insert({row, col});
    }
  }

  const auto startCluster = std::chrono::steady_clock::now();
  auto clusters =
      std::map<GridNode<gridWidth>, std::set<GridNode<gridWidth>>>{};

  for (int color = 0; !nodesWithoutComponent.empty();
       color     = (color + 1) % maxColors) {
    // Extract the next node that forms a component.
    const auto nodeIt = nodesWithoutComponent.begin();
    const auto newC   = *nodeIt;
    nodesWithoutComponent.erase(nodeIt);

    auto newCluster = std::set<GridNode<gridWidth>>{newC};

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

    auto [component, inserted] = clusters.insert({newC, std::move(newCluster)});
    if (!inserted) {
      throw std::logic_error{"Node already exists in different cluster."};
    }
    clusterColor[newC] = color;
  }
  const auto endCluster = std::chrono::steady_clock::now();

  auto clusteredNodes   = 0;
  for (const auto &[startNode, elements] : clusters) {
    std::cout << "Cluster at (" << startNode.row << ", " << startNode.column
              << ") with " << elements.size() << " elements.\n";
    clusteredNodes += int(elements.size());
  }
  std::cout << "Cluster Graph Computation took "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endPower -
                                                                     startPower)
                   .count()
            << " ms\n";
  std::cout << "Cluster Computation took "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   endCluster - startCluster)
                   .count()
            << " us\n";
  if (clusteredNodes != matrixSize) {
    throw std::logic_error{"All nodes must be part of a cluster and always sum "
                           "up to matrix size. Graph Invariant Violated"};
  }

  std::ofstream graphOutput{argv[2]};
  for (const auto &[initialNode, elements] : clusters) {
    for (const auto &node : elements) {
      graphOutput << node.column << " " << node.row << " "
                  << clusterColor.at(initialNode) << "\n";
    }
  }
  return 0;
}
