#pragma once
#include "Envelope.h"
#include "LengthCounter.h"
#include "Sequencer.h"
#include "Sweeper.h"

class PulseChannel {
 public:
  PulseChannel(bool firstChannel);
  Envelope mEnvelope;
  Sequencer mSequencer;
  Sweeper mSweeper;
  LengthCounter mLengthCounter;
  uint8_t output = 0;

  void Clock(bool quarter, bool half);

 private:
  int mClock = 0;
};
