add_executable(calculate_fibonacci.x calculate_fibonacci.cpp)
target_link_libraries(calculate_fibonacci.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
target_compile_features(calculate_fibonacci.x PUBLIC cxx_std_20)

add_executable(find_prime_numbers.x find_prime_numbers.cpp)
target_link_libraries(find_prime_numbers.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
target_compile_features(find_prime_numbers.x PUBLIC cxx_std_20)

add_executable(collatz_chain.x collatz_chain.cpp)
target_link_libraries(collatz_chain.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
target_compile_features(collatz_chain.x PUBLIC cxx_std_20)

add_executable(percolation_power.x percolation_power.cpp)
target_link_libraries(percolation_power.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
target_compile_features(percolation_power.x PUBLIC cxx_std_20)

add_executable(shortest_path.x shortest_path.cpp)
target_link_libraries(shortest_path.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
target_compile_features(shortest_path.x PUBLIC cxx_std_20)

add_executable(toy_rsa.x toy_rsa.cpp)
target_link_libraries(toy_rsa.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
target_compile_features(toy_rsa.x PUBLIC cxx_std_20)

add_executable(sha256sum.x sha256sum.cpp)
target_compile_features(sha256sum.x PUBLIC cxx_std_20)
