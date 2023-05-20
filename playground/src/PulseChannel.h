#pragma once
#include "Envelope.h"
#include "Oscilator.h"
#include "Sequencer.h"

class PulseChannel {
 public:
  Envelope mEnvelope;
  Sequencer mSequencer;
  Oscilator mOscilator;
  double output = 0;

  void Clock();

 private:
  double mClock = 0;
};
