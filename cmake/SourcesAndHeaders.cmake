set(sources
    lib/container/BitVector.cpp
    lib/math/BigUInt.cpp
)

set(exe_sources
    bin/calculate_fibbonacci.cpp
    ${sources}
)

set(headers
    include/jt-computing/container/BitVector.hpp
    include/jt-computing/math/BigUInt.hpp
    include/jt-computing/math/Concepts.hpp
    include/jt-computing/math/NaturalNumberAlgorithms.hpp
)

set(test_sources
    lib/container/BitVector.cpp
    lib/math/BigUInt.cpp
    lib/math/NaturalNumberAlgorithms.cpp
)
