#include "PulseChannel.h"
#include <iostream>

void PulseChannel::Clock() {
  mEnvelope.Clock();
  mSequencer.Clock();

  output = 0;
  if (mSequencer.output != 0) {
    output = mEnvelope.output * mOscilator.Sample(mClock);
  }
  mClock += 1.0 / 14833.5;
  // std::cout << mClock << "\n";
}
