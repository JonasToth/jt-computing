add_executable(calculate_fibonacci.x calculate_fibonacci.cpp)
target_link_libraries(calculate_fibonacci.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

add_executable(find_prime_numbers.x find_prime_numbers.cpp)
target_link_libraries(find_prime_numbers.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

add_executable(collatz_chain.x collatz_chain.cpp)
target_link_libraries(collatz_chain.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

add_executable(percolation_power.x percolation_power.cpp)
target_link_libraries(percolation_power.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
