set(module_sources
    lib/core/Core.cppm
    lib/core/Constants.cpp
    lib/core/Types.cpp

    lib/container/Container.cppm
    lib/container/BitVector.cpp

    lib/crypto/Crypto.cppm
    lib/crypto/Concepts.cpp
    lib/crypto/Sha256.cpp
    lib/crypto/TextbookRSA.cpp

    lib/math/Math.cppm
    lib/math/BigInt.cpp
    lib/math/BigUInt.cpp
    lib/math/BigUIntImpl.cpp
    lib/math/Concepts.cpp
    lib/math/FixedSquareMatrix.cpp
    lib/math/GenericPower.cpp
    lib/math/ModularArithmetic.cpp
    lib/math/NaturalN.cpp
    lib/math/NaturalNImpl.cpp
    lib/math/NaturalNumberAlgorithms.cpp
    lib/math/NumberIO.cpp
    lib/math/Rational.cpp
    lib/math/Operations.cpp
)

set(test_sources
    lib/container/BitVector.cpp
    lib/crypto/Sha256.cpp
    lib/crypto/TextbookRSA.cpp
    lib/math/BigUInt.cpp
    lib/math/BigInt.cpp
    lib/math/GenericPower.cpp
    lib/math/ModularArithmetic.cpp
    lib/math/NaturalN.cpp
    lib/math/NaturalNumberAlgorithms.cpp
    lib/math/Rational.cpp
)
