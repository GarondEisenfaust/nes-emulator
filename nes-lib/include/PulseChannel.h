#pragma once
#include "Envelope.h"
#include "LengthCounter.h"
#include "Sequencer.h"
#include "Sweeper.h"

class PulseChannel {
 public:
  PulseChannel(bool isSecondChannel);
  Envelope mEnvelope;
  Sequencer mSequencer;
  Sweeper mSweeper;
  LengthCounter mLengthCounter;
  uint8_t output = 0;

  void Clock(bool quarter, bool half);
  void Write(uint16_t addr, uint8_t data);

 private:
  bool mIsSecondChannel;
  int mClock = 0;
};
