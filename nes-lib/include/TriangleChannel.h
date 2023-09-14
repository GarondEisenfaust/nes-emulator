#pragma once
#include "Divider.h"
#include "LengthCounter.h"
#include <array>
#include <cstdint>

class TriangleChannel {
 public:
  TriangleChannel();
  ~TriangleChannel();

  void UpdateState(uint16_t addr, uint8_t data);
  void Clock(bool quarter, bool half);
  uint8_t output = 0;
  LengthCounter mLengthCounter;

 private:
  Divider mDivider;
  bool mCounterControlFlag;
  uint8_t mCounterReloadValue;
  uint8_t mCounter;
  bool mCounterReload;
  std::array<uint8_t, 32> mSequence;
  int mSequenceIndex = 0;
};
