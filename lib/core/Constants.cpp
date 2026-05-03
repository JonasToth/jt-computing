module;

#include <climits>

export module jt.Core:Constants;

export namespace jt {
constexpr int EXIT_FAILURE = 1;
constexpr int EXIT_SUCCESS = 0;

constexpr int BitsPerByte = CHAR_BIT;
} // namespace jt
