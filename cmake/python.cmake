set (VENV_PATH ${CMAKE_BINARY_DIR}/venv)
execute_process(COMMAND python -m venv ${VENV_PATH})
execute_process(COMMAND source ${VENV_PATH}/bin/activate)
execute_process(COMMAND pip install -r ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt)
