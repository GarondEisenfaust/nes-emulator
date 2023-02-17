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
  PRIVATE util
  PRIVATE nes-lib
  PRIVATE CONAN_PKG::glfw
  PRIVATE CONAN_PKG::glew
  PRIVATE CONAN_PKG::glm
  PRIVATE CONAN_PKG::fmt
  PRIVATE CONAN_PKG::imgui)

message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
message(${PROJECT_BINARY_DIR}/shaders)

add_custom_target(
  copy-shaders ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
  COMMENT "Copying shaders into the output directory")

add_dependencies(${PROJECT_NAME} copy-shaders)
