add_executable(calculate_fibonacci.x calculate_fibonacci.cpp)
target_link_libraries(calculate_fibonacci.x PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
