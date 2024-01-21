function(generate_shader_headers SHADER_DIR SHADER_HEADER_DIR RESULT_HEADERS)
  set(GENERATE_SHADER_HEADERS_SCRIPT
      ${CMAKE_SOURCE_DIR}/scripts/generate-shader-headers.py)

  file(GLOB SHADER_FILES "${SHADER_DIR}/*")
  foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WLE)
    set(SHADER_HEADER ${SHADER_HEADER_DIR}${SHADER_NAME}Shader.h)
    list(APPEND SHADER_HEADERS ${SHADER_HEADER})
    add_custom_command(
      DEPENDS ${SHADER}
      OUTPUT ${SHADER_HEADER}
      COMMAND ${PYTHON_CMD} ${GENERATE_SHADER_HEADERS_SCRIPT} ${SHADER}
              ${SHADER_HEADER}
      COMMENT "Generating ${SHADER_HEADER} from ${SHADER}")
  endforeach()
  set(${RESULT_HEADERS}
      ${SHADER_HEADERS}
      PARENT_SCOPE)
endfunction()
