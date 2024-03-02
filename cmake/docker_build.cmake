function(docker_build_target build_type)
  string(TOLOWER ${build_type} LOWER_CASE_NAME)
  add_custom_target(
    build-docker-${LOWER_CASE_NAME}
    COMMAND python ${CMAKE_SOURCE_DIR}/scripts/build-in-docker-container.py
            ${CMAKE_SOURCE_DIR} ${build_type})
endfunction()

docker_build_target(Release)
docker_build_target(Debug)
