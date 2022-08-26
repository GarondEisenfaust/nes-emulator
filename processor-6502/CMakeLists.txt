cmake_minimum_required(VERSION 3.14)

create_library()
target_link_libraries(${PROJECT_NAME} PUBLIC util)
