#pragma once

#include "jt-computing/Types.hpp"
#include "jt-computing/math/AlgebraConcepts.hpp"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <iostream>
#include <memory>

namespace jt::math {

template <std::regular T, i32 N, typename Plus = std::plus<T>,
          typename Times = std::multiplies<T>>
  requires(SemiRing<Plus, Times, T>)
class FixedSquareMatrix {
public:
  static_assert(N > 0, "Matrix must have positive dimension");

  FixedSquareMatrix(Plus plus, Times times)
      : _data{std::make_unique<T[]>(N * N)}, _plus{std::move(plus)},
        _times{std::move(times)} {
    assert(_data.get() != nullptr && "Bad Alloc must have been thrown");
    std::fill_n(_data.get(), N * N, identity_element(_plus));
  }

  /// Default construction of the matrix with all zeros.
  FixedSquareMatrix() : FixedSquareMatrix(Plus{}, Times{}) {}

  /// Explicit value initialization.
  FixedSquareMatrix(std::initializer_list<T> args)
      : _data{std::make_unique<T[]>(N * N)} {
    if (args.size() != N * N) {
      throw std::invalid_argument{"Mismatched dimension"};
    }
    std::copy(args.begin(), args.end(), _data.get());
  }
  /// Create Zero or Identity Matrix of neutral elements.
  explicit FixedSquareMatrix(i32 i, Plus plus = {}, Times times = {})
      : FixedSquareMatrix(std::move(plus), std::move(times)) {
    assert(i == 0 || i == 1);
    if (i == 1) {
      for (int diagonal = 0; diagonal < N; ++diagonal) {
        (*this)(diagonal, diagonal) = identity_element(_times);
      }
    }
  }

  FixedSquareMatrix(const FixedSquareMatrix &other)
      : _data{std::make_unique<T[]>(N * N)} {
    std::copy_n(other._data.get(), N * N, _data.get());
  }
  FixedSquareMatrix(FixedSquareMatrix &&other) noexcept = default;

  FixedSquareMatrix &operator=(const FixedSquareMatrix &other) {
    if (&other == this) {
      return *this;
    }
    std::copy_n(other._data.get(), N * N, _data.get());
  }
  FixedSquareMatrix &operator=(FixedSquareMatrix &&other) noexcept = default;
  ~FixedSquareMatrix() noexcept                                    = default;

  T &operator()(i32 i, i32 j) {
    assert(i >= 0);
    assert(i < N);
    assert(j >= 0);
    assert(j < N);

    return _data[static_cast<usize>(i * N + j)];
  }
  const T &operator()(i32 i, i32 j) const {
    assert(i >= 0);
    assert(i < N);
    assert(j >= 0);
    assert(j < N);

    return _data[static_cast<usize>(i * N + j)];
  }

  friend bool operator==(const FixedSquareMatrix &a,
                         const FixedSquareMatrix &b) noexcept {
    return std::equal(a._data.get(), a._data.get() + N * N, b._data.get());
  }
  friend bool operator!=(const FixedSquareMatrix &a,
                         const FixedSquareMatrix &b) noexcept {
    return !(a == b);
  }

  friend FixedSquareMatrix operator*(const FixedSquareMatrix &a,
                                     const FixedSquareMatrix &b) {
    auto result = FixedSquareMatrix{};
    for (i32 i = 0; i < N; ++i) {
      for (i32 j = 0; j < N; ++j) {
        for (i32 k = 0; k < N; ++k) {
          result(i, j) = a._plus(result(i, j), a._times(a(i, k), b(k, j)));
        }
      }
    }
    return result;
  }

  template <typename AA, i32 NN, typename PPlus, typename TTimes>
  friend std::ostream &
  operator<<(std::ostream &os,
             const FixedSquareMatrix<AA, NN, PPlus, TTimes> &m);

private:
  std::unique_ptr<T[]> _data;
  Plus _plus;
  Times _times;
};

template <typename AA, i32 NN, typename PPlus, typename TTimes>
std::ostream &operator<<(std::ostream &os,
                         const FixedSquareMatrix<AA, NN, PPlus, TTimes> &m) {
  for (i32 i = 0; i < NN; ++i) {
    for (i32 j = 0; j < NN; ++j) {
      os << m(i, j) << ", ";
    }
    os << std::endl;
  }
  return os;
}

} // namespace jt::math
