cmake_minimum_required(VERSION 3.14)

set_defaults()

set(SHADER_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders)
set(SHADER_HEADER_DIR ${CMAKE_BINARY_DIR}/shaders/)
generate_shader_headers(${SHADER_DIR} ${SHADER_HEADER_DIR} SHADER_HEADERS)

set(SOURCE_FILES
    ${SOURCE_FILES}
    ${SHADER_HEADERS}
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.cpp
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp)

set(INCLUDES ${INCLUDES} ${CMAKE_BINARY_DIR}/bindings/ ${SHADER_HEADER_DIR})

create_executable()

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE util
  PRIVATE nes-lib
  PRIVATE CONAN_PKG::glfw
  PRIVATE CONAN_PKG::glew
  PRIVATE CONAN_PKG::glm
  PRIVATE CONAN_PKG::fmt
  PRIVATE CONAN_PKG::imgui)
