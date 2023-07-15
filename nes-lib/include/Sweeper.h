#pragma once
#include "Divider.h"
#include <cstdint>

class Sweeper {
 public:
  Sweeper(bool onesComplement);
  bool enable;
  bool negate;
  bool mOnesComplement;
  uint8_t shiftCount;
  Divider mDivider;
  bool mMute;

  uint16_t Clock(uint16_t timerPeriod);
  void UpdateState(uint8_t data);
  bool ShouldMute();
};
