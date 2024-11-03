if (NOT ANDROID_ABI OR NOT CMAKE_BUILD_TYPE)
    return()
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(CMAKE_BUILD_DIR_NAME "android-armv8-${CMAKE_BUILD_TYPE_LOWER}")
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(CMAKE_BUILD_DIR_NAME "android-armv7-${CMAKE_BUILD_TYPE_LOWER}")
elseif(${ANDROID_ABI} STREQUAL "x86")
    set(CMAKE_BUILD_DIR_NAME "android-x86-${CMAKE_BUILD_TYPE_LOWER}")
elseif(${ANDROID_ABI} STREQUAL "x86_64")
    set(CMAKE_BUILD_DIR_NAME "android-x86_64-${CMAKE_BUILD_TYPE_LOWER}")
endif()

include("${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_DIR_NAME}/generators/conan_toolchain.cmake")
