set(ROM_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/roms")

function(DOWNLOAD_IF_MISSING)
  if(NOT EXISTS ${ARGV1})
    file(DOWNLOAD ${ARGV0} ${ARGV1} TLS_VERIFY ON)
  endif()
endfunction()

download_if_missing(
  "https://forums.nesdev.org/download/file.php?id=4043&sid=1dca5dff5025b495ca377c65fe999d78"
  ${ROM_DIR}/color_test.nes)
download_if_missing("http://nickmass.com/images/nestest.nes"
                    ${ROM_DIR}/nestest.nes)
