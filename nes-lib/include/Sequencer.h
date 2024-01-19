#pragma once
#include "Divider.h"
#include <cstdint>

class Sequencer {
 public:
  uint32_t sequence = 0x00000000;
  uint32_t reloadSequence = 0x00000000;
  uint8_t output = 0x00;
  Divider mDivider;

  void Clock();
  void Reload();
};
