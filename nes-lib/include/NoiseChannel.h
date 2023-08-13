#pragma once
#include "Envelope.h"
#include "LengthCounter.h"
#include <array>
#include <cstdint>

class NoiseChannel {
 public:
  NoiseChannel();
  void UpdateState(uint16_t addr, uint8_t data);
  void Clock(bool quarter, bool half);
  uint8_t output = 0;

 private:
  Envelope mEnvelope;
  LengthCounter mLengthCounter;
  std::array<int, 16> mTimerTable;
  uint16_t shiftRegister = 0x01;
  bool mode;
  int mTimer;

  bool ShiftRegister();
};
