#pragma once
#include "Envelope.h"
#include "LengthCounter.h"
#include "Oscilator.h"
#include "Sweeper.h"

class PulseChannel {
 public:
  PulseChannel(bool isSecondChannel);
  Envelope mEnvelope;
  Divider mDivider;
  Sweeper mSweeper;
  LengthCounter mLengthCounter;
  Oscilator mOscilator;
  double output = 0;

  void Clock(bool quarter, bool half, double globalTime);
  void Write(uint16_t addr, uint8_t data);

 private:
  const bool mIsSecondChannel;
  const int mAddressOffset;
  int mClock = 0;
};
