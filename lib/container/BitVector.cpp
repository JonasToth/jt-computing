#include "jt-computing/container/BitVector.hpp"

#include <algorithm>
#include <cassert>

namespace jt::container {

BitVector::BitVector(usize length, bool initialValue)
    : _data(length, initialValue ? u8{1} : u8{0}) {}

void BitVector::normalize() {
  const auto itFirstOne = std::find(_data.crbegin(), _data.crend(), u8{1});
  _data.erase(_data.begin() + std::distance(itFirstOne, _data.crend()),
              _data.end());
}

BitVector &BitVector::operator<<=(int i) {
  assert(i > 0);

  const auto sizeBefore [[maybe_unused]] = _data.size();
  _data.insert(_data.begin(), usize(i), u8{0});

  assert(sizeBefore + usize(i) == size());
  return *this;
}

BitVector &BitVector::operator>>=(int i) {
  assert(i > 0);
  assert(usize(i) < size());

  const auto sizeBefore [[maybe_unused]] = _data.size();
  _data.erase(_data.begin(), _data.begin() + i);

  assert(sizeBefore - usize(i) == size());
  return *this;
}

} // namespace jt::container
