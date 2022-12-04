#include "Util.h"
#include <boost/dll/runtime_symbol_info.hpp>
#include <random>
#include <string>

namespace Util {

uint8_t SetFlag(uint8_t& toSetIn, FLAGS6502 flag, std::function<bool(void)> value) {
  if (value()) {
    toSetIn |= flag;
  } else {
    toSetIn &= ~flag;
  }
  return toSetIn;
}

uint8_t SetFlag(uint8_t& toSetIn, FLAGS6502 flag, bool value) {
  return SetFlag(toSetIn, flag, [value]() { return value; });
}

template <>
std::stringstream IntToHex(uint8_t num) {
  return IntToHex<uint16_t>(num);
}

float GetRandomFloat() { return rand() / static_cast<float>(RAND_MAX); }

std::string GetExecutableDirectory() { return std::string(boost::dll::program_location().parent_path().c_str()); }

}  // namespace Util
