#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>

namespace jt::math {

template <std::regular T, int N> class FixedSquareMatrix {
public:
  static_assert(N > 0, "Matrix must have positive dimension");

  FixedSquareMatrix() : _data{std::make_unique<T[]>(N * N)} {
    assert(_data.get() != nullptr && "Bad Alloc must have been thrown");
    std::fill_n(_data.get(), N * N, T{0U});
  }

  FixedSquareMatrix(std::initializer_list<T> args)
      : _data{std::make_unique<T[]>(N * N)} {
    if (args.size() != N * N) {
      throw std::invalid_argument{"Mismatched dimension"};
    }
    std::copy(args.begin(), args.end(), _data.get());
  }
  /// Create Zero or Identity Matrix for neutral elements.
  explicit FixedSquareMatrix(int i) : FixedSquareMatrix() {
    assert(i == 0 || i == 1);
    if (i == 1) {
      for (int diagonal = 0; diagonal < N; ++diagonal) {
        (*this)(diagonal, diagonal) = T{1U};
      }
    }
  }

  FixedSquareMatrix(const FixedSquareMatrix<T, N> &other)
      : _data{std::make_unique<T[]>(N * N)} {
    std::copy_n(other._data.get(), N * N, _data.get());
  }
  FixedSquareMatrix(FixedSquareMatrix<T, N> &&other) noexcept = default;

  FixedSquareMatrix &operator=(const FixedSquareMatrix<T, N> &other) {
    if (&other == this) {
      return *this;
    }
    std::copy_n(other._data.get(), N * N, _data.get());
  }
  FixedSquareMatrix &
  operator=(FixedSquareMatrix<T, N> &&other) noexcept = default;
  ~FixedSquareMatrix() noexcept                       = default;

  T &operator()(int i, int j) {
    assert(i >= 0);
    assert(i < N);
    assert(j >= 0);
    assert(j < N);

    return _data[std::size_t(i) * N + std::size_t(j)];
  }
  const T &operator()(int i, int j) const {
    assert(i >= 0);
    assert(i < N);
    assert(j >= 0);
    assert(j < N);

    return _data[std::size_t(i) * N + std::size_t(j)];
  }

  template <std::regular TT, int NN>
  friend bool operator==(const FixedSquareMatrix<TT, NN> &a,
                         const FixedSquareMatrix<TT, NN> &b) noexcept;

private:
  std::unique_ptr<T[]> _data;
};

template <std::regular T, int N>
FixedSquareMatrix<T, N> operator*(const FixedSquareMatrix<T, N> &a,
                                  const FixedSquareMatrix<T, N> &b) {
  auto result = FixedSquareMatrix<T, N>(0U);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        result(i, j) += a(i, k) * b(k, j);
      }
    }
  }
  return result;
}

template <std::regular T, int N>
bool operator==(const FixedSquareMatrix<T, N> &a,
                const FixedSquareMatrix<T, N> &b) noexcept {
  return std::equal(a._data.get(), a._data.get() + N * N, b._data.get());
}
template <std::regular T, int N>
bool operator!=(const FixedSquareMatrix<T, N> &a,
                const FixedSquareMatrix<T, N> &b) {
  return !(a == b);
}

template <std::regular T, int N>
std::ostream &operator<<(std::ostream &os, const FixedSquareMatrix<T, N> &m) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      os << m(i, j) << ", ";
    }
    os << std::endl;
  }
  return os;
}

} // namespace jt::math
