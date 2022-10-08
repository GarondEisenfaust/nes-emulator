cmake_minimum_required(VERSION 3.14)

create_executable()
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC processor-6502
  PUBLIC util)
