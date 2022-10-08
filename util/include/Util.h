#pragma once
#include "FLAGS6502.h"
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Util {
uint8_t SetFlag(uint8_t& toSetIn, FLAGS6502 flag, bool value);
uint8_t SetFlag(uint8_t& toSetIn, FLAGS6502 flag, std::function<bool(void)> value);

template <typename T>
std::stringstream IntToHex(T num) {
  std::stringstream asHex;
  asHex << std::setfill('0') << std::setw(sizeof(T)) << std::hex << num;
  return asHex;
}

template <>
std::stringstream IntToHex(uint8_t num);

}  // namespace Util
