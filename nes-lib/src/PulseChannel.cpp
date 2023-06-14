#include "PulseChannel.h"
#include <iostream>

void PulseChannel::Clock() {
  // mEnvelope.Clock();
  mSequencer.Clock();

  output = mSequencer.output;
  if (output != 0) {
    auto p = 0;
  }
  // if (mSequencer.output != 0) {
  //   output = mEnvelope.output * mOscilator.Sample(mClock);
  // }
  // mClock += 1.0 / 14833.5;
  // std::cout << mClock << "\n";
}
