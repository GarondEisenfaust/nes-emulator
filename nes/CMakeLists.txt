cmake_minimum_required(VERSION 3.14)

set_defaults()

set(IMGUI_INCLUDES ${CMAKE_BINARY_DIR}/conan-output/bindings/)
set(IMGUI_SOURCE ${CMAKE_BINARY_DIR}/conan-output/bindings/imgui_impl_glfw.cpp
                 ${CMAKE_BINARY_DIR}/conan-output/bindings/imgui_impl_opengl3.cpp)

set(SOURCE_FILES ${SOURCE_FILES} ${IMGUI_SOURCE})
set(INCLUDES ${INCLUDES} ${CMAKE_BINARY_DIR}/bindings/ ${IMGUI_INCLUDES})

create_executable()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PUBLIC -msse2 -mavx2)

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(miniaudio REQUIRED)

target_link_libraries(
${PROJECT_NAME}
  PRIVATE nes-lib
  PRIVATE rendering
  PRIVATE miniaudio::miniaudio
  PRIVATE glfw
  PRIVATE glad::glad
  PRIVATE imgui::imgui)

set(ROM_SOURCE_DIR ${CMAKE_SOURCE_DIR}/roms)
set(ROM_TARGET_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/roms)
if((NOT EXISTS "${ROM_TARGET_DIR}") AND (EXISTS "${ROM_SOURCE_DIR}"))
  file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  file(COPY ${ROM_SOURCE_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
