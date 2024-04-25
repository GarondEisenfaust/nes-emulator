#pragma once
#include "Divider.h"
#include "LengthCounter.h"
#include "TriangleWaveGenerator.h"
#include <array>
#include <cstdint>

class TriangleChannel {
 public:
  TriangleChannel();
  ~TriangleChannel();

  void Write(uint16_t addr, uint8_t data);
  void Clock(bool quarter, bool half, double globalTime);
  double output = 0;
  LengthCounter mLengthCounter;

 private:
  Divider mDivider;
  TriangleWaveGenerator mTriangleWaveGenerator;
  double mFrequency = 0;
  bool mCounterControlFlag;
  uint8_t mCounterReloadValue;
  uint8_t mCounter;
  bool mCounterReload;
  void UpdateFrequency();
};
