#include "Util.h"

namespace Util {
void SetFlag(uint8_t& toSetIn, FLAGS6502 flag, std::function<bool(void)> value) {
  if (value()) {
    toSetIn |= flag;
  } else {
    toSetIn &= ~flag;
  }
}

void SetFlag(uint8_t& toSetIn, FLAGS6502 flag, bool value) {
  SetFlag(toSetIn, flag, [value]() { return value; });
}
}  // namespace Util
