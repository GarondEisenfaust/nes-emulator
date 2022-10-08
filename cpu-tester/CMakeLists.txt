cmake_minimum_required(VERSION 3.14)

set_defaults()
set(SOURCE_FILES
    ${SOURCE_FILES}
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.h
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_glfw.cpp
    ${CMAKE_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp)

set(INCLUDES ${INCLUDES} ${CMAKE_BINARY_DIR}/bindings/)

create_executable()
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE processor-6502
  PRIVATE util
  PRIVATE CONAN_PKG::imgui
  PRIVATE CONAN_PKG::glfw
  PRIVATE CONAN_PKG::glew)

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
