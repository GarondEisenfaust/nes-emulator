cmake_minimum_required(VERSION 3.14)
set_defaults()
create_library(STATIC)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
