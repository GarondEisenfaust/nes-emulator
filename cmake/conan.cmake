list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  set(CMAKE_CONAN_URL
      "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
  )
  message(STATUS "Downloading conan.cmake from ${CMAKE_CONAN_URL}")
  file(DOWNLOAD "${CMAKE_CONAN_URL}" "${CMAKE_BINARY_DIR}/conan.cmake"
       TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)
conan_cmake_run(
  CONANFILE
  ${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD
  missing
  SETTINGS
  ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
