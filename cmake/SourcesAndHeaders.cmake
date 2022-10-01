set(sources
    lib/container/BitVector.cpp
    lib/math/BigUInt.cpp
    lib/math/BigInt.cpp
    lib/math/Rational.cpp
)

set(exe_sources
    ${sources}
)

set(headers
    include/jt-computing/container/BitVector.hpp
    include/jt-computing/math/BigInt.hpp
    include/jt-computing/math/BigUInt.hpp
    include/jt-computing/math/Concepts.hpp
    include/jt-computing/math/NaturalNumberAlgorithms.hpp
    include/jt-computing/math/Rational.hpp
)

set(test_sources
    lib/container/BitVector.cpp
    lib/math/BigUInt.cpp
    lib/math/BigInt.cpp
    lib/math/NaturalNumberAlgorithms.cpp
    lib/math/Rational.cpp
)
