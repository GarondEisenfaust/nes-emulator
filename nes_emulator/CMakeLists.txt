cmake_minimum_required(VERSION 3.14)
set_defaults()

set(SHADER_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders/)
set(SHADER_HEADER_DIR ${CMAKE_BINARY_DIR}/shaders/)
generate_shader_headers(${SHADER_DIR} ${SHADER_HEADER_DIR} SHADER_HEADERS)

set(SOURCE_FILES ${SOURCE_FILES})

set(INCLUDES ${INCLUDES} ${SHADER_HEADER_DIR})

create_library(SHARED)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PUBLIC -msse2 -mavx2)

find_package(game-activity REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME}
        PUBLIC game-activity::game-activity
        PUBLIC common
        PUBLIC EGL
        PUBLIC jnigraphics
        PUBLIC android
        PUBLIC log
        PUBLIC nes-lib)
