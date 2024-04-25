#pragma once
#include "Envelope.h"
#include "LengthCounter.h"
#include "PulseWaveGenerator.h"
#include "Sweeper.h"

class PulseChannel {
 public:
  PulseChannel(bool isSecondChannel);
  Envelope mEnvelope;
  Divider mDivider;
  Sweeper mSweeper;
  LengthCounter mLengthCounter;
  PulseWaveGenerator mPulseWaveGenerator;
  double output = 0;

  void Clock(bool quarter, bool half, double globalTime);
  void Write(uint16_t addr, uint8_t data);

 private:
  const bool mIsSecondChannel;
  const int mAddressOffset;
  int mClock = 0;
  double mFrequency = 0;
  void UpdateFrequency(uint16_t period);
};
