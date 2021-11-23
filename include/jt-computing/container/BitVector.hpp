#ifndef BITVECTOR_HPP_FIVX3TX5
#define BITVECTOR_HPP_FIVX3TX5

#include "jt-computing/Types.hpp"
#include <climits>
#include <concepts>
#include <iostream>
#include <vector>

namespace jt::container {
class BitVector {
public:
  BitVector() = default;
  ~BitVector() = default;
  BitVector(const BitVector &) = default;
  BitVector(BitVector &&) = default;
  BitVector &operator=(const BitVector &) = default;
  BitVector &operator=(BitVector &&) = default;

  /// Construct a @c BitVector that has enough bits to represent @c value and
  /// assign @c values bit pattern to the individual bits.
  explicit BitVector(std::unsigned_integral auto value);

  /// Construct a @c BitVector with initial capacity of at least @c length bits.
  BitVector(usize length, bool initialValue);

  /// Return the underlying capacity of bits. This is a multiple of an integer
  /// type bits.
  [[nodiscard]] usize capacity() const noexcept { return _data.capacity(); }

  /// Return the current number of bits managed by the vector.
  [[nodiscard]] usize size() const noexcept { return _data.size(); }

  /// Return the bit at any position.
  [[nodiscard]] bool get(usize index) const { return _data.at(index) == u8{1}; }

  /// Provide access to the bit at any position.
  void set(usize index, bool value) { _data.at(index) = value ? u8{1} : u8{0}; }

  /// Append a new bit to the end of the vector.
  void push_back(bool bit) { _data.push_back(bit ? u8{1} : u8{0}); }

  /// Removes leading zeros from the @c BitVector.
  void normalize();

  /// Shift all bits left by @c i position(s). The new bit(s) is/are inserted at
  /// index 0 and have the value @c false. All old bits increase their index by
  /// @c i.
  /// @pre i > 0
  /// @post VectorBefore.size() + i == VectorAfter.size()
  BitVector &operator<<=(int i);

  /// Shift all bits right by @c i position(s). No new bits are inserted and @c
  /// i bits are dropped.
  /// @pre i > 0 && i < size()
  /// @post VectorBefore.size() - i == VectorAfter.size()
  BitVector &operator>>=(int i);

private:
  std::vector<u8> _data;
};

BitVector::BitVector(std::unsigned_integral auto value)
    : _data(sizeof(value) * CHAR_BIT, u8{0}) {
  for (u32 i = 0U; i < CHAR_BIT * sizeof(value); ++i) {
    const bool bitFromValue = value & (static_cast<decltype(value)>(1U) << i);
    set(i, bitFromValue);
  }
}
} // namespace jt::container

#endif /* end of include guard: BITVECTOR_HPP_FIVX3TX5 */
