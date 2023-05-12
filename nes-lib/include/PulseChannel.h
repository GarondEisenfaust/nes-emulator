#pragma once
#include "Envelope.h"
#include "Oscilator.h"
#include "Sequencer.h"

class PulseChannel {
 public:
  Envelope mEnvelope;
  Sequencer mSequencer;
  Oscilator mOscilator;
  uint8_t output = 0;

  void Clock();

 private:
  int mClock = 0;
};
