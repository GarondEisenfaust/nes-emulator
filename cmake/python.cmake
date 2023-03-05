set(PIP_CMD ${VENV_PATH}/bin/pip)
set(PYTHON_CMD ${VENV_PATH}/bin/python)
set(REQUIREMENTS_TXT ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt)

if(NOT EXISTS ${PIP_CMD})
  message("Creating Python venv at ${VENV_PATH}")
  execute_process(COMMAND python -m venv ${VENV_PATH})
endif()

message("Installing Python requirements from ${REQUIREMENTS_TXT}")
execute_process(COMMAND ${PIP_CMD} install -r ${REQUIREMENTS_TXT} --upgrade)
