cmake_minimum_required(VERSION 3.14)

set_defaults()

set(SHADER_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders)
set(SHADER_HEADER_DIR ${CMAKE_BINARY_DIR}/shaders/)
generate_shader_headers(${SHADER_DIR} ${SHADER_HEADER_DIR} SHADER_HEADERS)

set(SOURCE_FILES ${SOURCE_FILES} ${SHADER_HEADERS})
set(INCLUDES ${INCLUDES} ${CMAKE_BINARY_DIR}/bindings/ ${SHADER_HEADER_DIR})

create_library(STATIC)
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC nes-lib
)

if(${ANDROID})
  target_link_libraries(${PROJECT_NAME} PUBLIC GLESv3)
endif()
