#include "ArgumentParser.h"
#include <cstring>
#include <stdexcept>

NesConfig::SignalDecoder ResolveDecoder(const char* value) {
  if (strcmp(value, "ntsc") == 0) {
    return NesConfig::SignalDecoder::Ntsc;
  }
  if (strcmp(value, "ntsc-gpu") == 0) {
    return NesConfig::SignalDecoder::NtscGpu;
  }
  if (strcmp(value, "lookup") == 0) {
    return NesConfig::SignalDecoder::LookupTable;
  }
  std::string error = std::string(value) + " is not a valid signal decoder type!";
  throw std::runtime_error(error);
}

NesConfig ArgumentParser::Parse(int argc, char* argv[]) {
  NesConfig config;

  for (int i = 0; i < argc; i++) {
    const char* value = argv[i];
    if (strcmp(value, "--rom") == 0) {
      config.mRomPath = argv[++i];
    } else if (strcmp(value, "--rom-dir") == 0) {
      config.mRomDirPath = argv[++i];
    } else if (strcmp(value, "--decoder") == 0) {
      config.mDecoder = ResolveDecoder(argv[++i]);
    }
  }

  return config;
}
