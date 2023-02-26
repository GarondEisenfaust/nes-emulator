cmake_minimum_required(VERSION 3.14)

project(
  nes-emulator
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include cmake utilities
include(cmake/python.cmake)
include(cmake/macros.cmake)
include(cmake/conan.cmake)
include(cmake/download_roms.cmake)

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
  enable_testing()
endif()

add_subdirectory(nes)
add_subdirectory(playground)
add_subdirectory(nes-lib)
add_subdirectory(util)
