#pragma once

#ifdef __clang__
#  define PRE(...)
#  define POST(...)
#  define CONTRACT_ASSERT(...)
#else
#  define PRE(...) pre(__VA_ARGS__)
#  define POST(...) post(__VA_ARGS__)
#  define CONTRACT_ASSERT(...) contract_assert(__VA_ARGS__)
#endif
