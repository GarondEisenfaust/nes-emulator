cmake_minimum_required(VERSION 3.14)
set_defaults()
create_executable()

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE util
  PRIVATE processor-6502
  PRIVATE CONAN_PKG::glfw
  PRIVATE CONAN_PKG::glew
  PRIVATE CONAN_PKG::glm
  PRIVATE CONAN_PKG::boost
  PRIVATE CONAN_PKG::fmt)

message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
message(${PROJECT_BINARY_DIR}/shaders)

add_custom_target(copy-shaders ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
          ${CMAKE_CURRENT_LIST_DIR}/shaders
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
          COMMENT "Copying shaders into the output directory")

add_dependencies(${PROJECT_NAME} copy-shaders)
