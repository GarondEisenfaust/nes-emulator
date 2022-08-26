#pragma once
#include "FLAGS6502.h"
#include <cstdint>
#include <functional>

namespace Util {
void SetFlag(uint8_t& toSetIn, FLAGS6502 flag, bool value);
void SetFlag(uint8_t& toSetIn, FLAGS6502 flag, std::function<bool(void)> value);
}  // namespace Util
