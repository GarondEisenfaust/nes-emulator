#pragma once
#include "Divider.h"
#include <cstdint>

class Envelope {
 public:
  bool startFlag = false;
  bool loop = false;
  bool constantVolume = false;
  Divider divider;
  uint8_t decayLevelCounter = 0;
  uint8_t volume = 0;
  uint8_t output = 0;
  void Clock();
};