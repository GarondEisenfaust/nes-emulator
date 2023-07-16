#pragma once
#include "Envelope.h"
#include "Sequencer.h"
#include "Sweeper.h"

class PulseChannel {
 public:
  PulseChannel(bool firstChannel);
  Envelope mEnvelope;
  Sequencer mSequencer;
  Sweeper mSweeper;
  uint8_t output = 0;

  void Clock();

 private:
  int mClock = 0;
  const int ONE_PERIOD = 14833;
};
