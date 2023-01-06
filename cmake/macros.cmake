macro(set_defaults)
  set(CMAKE_CXX_STANDARD 20)

  # Output dirs
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

  # Source files
  file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")
  file(GLOB_RECURSE SHADERS "shaders/*.vert" "shaders/*.frag")

  # include files
  set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
  file(GLOB_RECURSE INCLUDE_FILES "${INCLUDES}/*.h")
endmacro()

macro(set_project_name)
  get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
  project(${PROJECT_NAME})
endmacro()

macro(create_executable)
  set_project_name()

  # Project name
  project(${PROJECT_NAME})

  add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} ${SHADERS})
  target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

  if(RUN_TESTS)
    create_tests()
  endif()
endmacro()

macro(create_library)
  set_project_name()

  # Project name
  project(${PROJECT_NAME})

  add_library(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} ${SHADERS})
  target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

  if(RUN_TESTS)
    create_tests()
  endif()
endmacro()

macro(create_tests)
  file(GLOB_RECURSE TEST_FILES "test/*.cpp" "test/*.h")
  if(TEST_FILES)
    set(TEST_NAME ${TEST_NAME} ${PROJECT_NAME}-test)
    add_executable(${TEST_NAME} ${TEST_FILES})
    target_link_libraries(
      ${TEST_NAME}
      PRIVATE ${PROJECT_NAME}
      PRIVATE CONAN_PKG::gtest)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 10)
  endif()
endmacro()
