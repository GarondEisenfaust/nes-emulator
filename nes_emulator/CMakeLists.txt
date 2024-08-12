cmake_minimum_required(VERSION 3.14)
set_defaults()
create_library(SHARED)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

find_package(game-activity REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME}
        PUBLIC game-activity::game-activity
        PUBLIC EGL
        PUBLIC GLESv3
        PUBLIC jnigraphics
        PUBLIC android
        PUBLIC log
        PUBLIC nes-lib)
