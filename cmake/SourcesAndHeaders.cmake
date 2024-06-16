set(sources
    lib/container/BitVector.cpp
    lib/crypto/Sha256.cpp
    lib/math/BigUInt.cpp
    lib/math/BigInt.cpp
    lib/math/NaturalN.cpp
    lib/math/RandomPrime.cpp
    lib/math/Rational.cpp
)

set(exe_sources
    ${sources}
)

set(headers
    include/jt-computing/container/BitVector.hpp
    include/jt-computing/crypto/Sha256.hpp
    include/jt-computing/crypto/TextbookRSA.hpp
    include/jt-computing/math/AlgebraConcepts.hpp
    include/jt-computing/math/BigInt.hpp
    include/jt-computing/math/BigUInt.hpp
    include/jt-computing/math/Concepts.hpp
    include/jt-computing/math/FixedSquareMatrix.hpp
    include/jt-computing/math/GenericPower.hpp
    include/jt-computing/math/ModularArithmetic.hpp
    include/jt-computing/math/NaturalN.hpp
    include/jt-computing/math/NaturalNumberAlgorithms.hpp
    include/jt-computing/math/RandomPrime.hpp
    include/jt-computing/math/Rational.hpp
)

set(test_sources
    lib/container/BitVector.cpp
    lib/crypto/Sha256.cpp
    lib/crypto/TextbookRSA.cpp
    lib/math/BigUInt.cpp
    lib/math/BigInt.cpp
    lib/math/GenericPower.cpp
    lib/math/MainTests.cpp
    lib/math/ModularArithmetic.cpp
    lib/math/NaturalN.cpp
    lib/math/NaturalNumberAlgorithms.cpp
    lib/math/RandomPrime.cpp
    lib/math/Rational.cpp
)
