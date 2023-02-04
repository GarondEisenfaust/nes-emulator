#include "Util.h"
#include <random>
#include <string>

namespace Util {
template <>
std::stringstream IntToHex(uint8_t num) {
  return IntToHex<uint16_t>(num);
}

float GetRandomFloat() { return rand() / static_cast<float>(RAND_MAX); }
}  // namespace Util
