cmake_minimum_required(VERSION 3.14)

project(
  cmake-template
  VERSION 1.0
  LANGUAGES CXX)

# Include cmake utilities
include(cmake/macros.cmake)
include(cmake/conan.cmake)

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
  enable_testing()
  find_package(GTest)
endif()

add_subdirectory(playground)
add_subdirectory(processor-6502)
add_subdirectory(util)
