cmake_minimum_required(VERSION 3.20)

#
# Project details
#

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

find_package(Catch2 2 REQUIRED)
verbose_message("Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

foreach(file ${test_sources})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name ${file}) 
  add_executable(${test_name}_Tests ${file})

  #
  # Set the compiler standard
  #

  target_compile_features(${test_name}_Tests PUBLIC cxx_std_20)

  #
  # Setup code coverage if enabled
  #

  if (${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
    verbose_message("Code coverage is enabled and provided with GCC.")
  endif()

  #
  # Load the desired unit testing framework
  #
  # Currently supported: Catch2.

  target_link_libraries(
    ${test_name}_Tests
    PUBLIC
      Catch2::Catch2
      ${CMAKE_PROJECT_NAME}
  )

  #
  # Add the unit tests
  #

  add_test(
    NAME
      ${test_name}
    COMMAND
      ${test_name}_Tests
  )
endforeach()

verbose_message("Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
