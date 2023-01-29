cmake_minimum_required(VERSION 3.14)

set_defaults()

set(INCLUDES ${INCLUDES} ${CMAKE_BINARY_DIR}/bindings/)

create_executable()
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::miniaudio)

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
