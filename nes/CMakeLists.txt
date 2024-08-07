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
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PUBLIC -msse2 -mavx2)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE nes-lib
  PRIVATE CONAN_PKG::miniaudio
  PRIVATE CONAN_PKG::glfw
  PRIVATE CONAN_PKG::glad
  PRIVATE CONAN_PKG::imgui)

set(ROM_SOURCE_DIR ${CMAKE_SOURCE_DIR}/roms)
set(ROM_TARGET_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/roms)
if((NOT EXISTS "${ROM_TARGET_DIR}") AND (EXISTS "${ROM_SOURCE_DIR}"))
  file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  file(COPY ${ROM_SOURCE_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
