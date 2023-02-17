cmake_minimum_required(VERSION 3.14)
set_defaults()
create_library()
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC util
  PRIVATE CONAN_PKG::lodepng)
