set(CONAN_HOME_SOURCE ${CMAKE_SOURCE_DIR}/.conan2/)
set(CONAN_HOME ${CMAKE_BINARY_DIR}/.conan2/)
set(CONAN_COMMAND ${VENV_PATH}/bin/conan)
set(CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.py)
set(ENV{CONAN_HOME} ${CONAN_HOME})

if(NOT EXISTS "${CONAN_HOME}")
  file(COPY ${CONAN_HOME_SOURCE} DESTINATION ${CONAN_HOME})
endif()

execute_process(COMMAND ${CONAN_COMMAND} install ${CMAKE_SOURCE_DIR} --build=missing --output-folder=${CMAKE_BINARY_DIR}/conan-output/)
