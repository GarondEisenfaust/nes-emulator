#pragma once
#include "FLAGS6502.h"
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace Util {

template <typename T>
std::stringstream IntToHex(T num) {
  std::stringstream asHex;
  asHex << std::setfill('0') << std::setw(sizeof(T)) << std::hex << num;
  return asHex;
}

template <>
std::stringstream IntToHex(uint8_t num);

float GetRandomFloat();
}  // namespace Util
