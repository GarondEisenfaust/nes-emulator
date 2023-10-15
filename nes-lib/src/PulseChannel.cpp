#include "PulseChannel.h"
#include <iostream>

PulseChannel::PulseChannel(bool firstChannel) : mSweeper(firstChannel) {}

void PulseChannel::Clock(bool quarter, bool half) {
  mSequencer.Clock();

  auto newTimer = 0;
  newTimer = mSweeper.Clock(mSequencer.mDivider.mPeriod);

  if (quarter) {
    mEnvelope.Clock();
  }

  if (half) {
    mLengthCounter.Clock();
    mSequencer.mDivider.SetPeriod(newTimer);
  }

  if (mSweeper.ShouldMute()) {
    output = 0;
    return;
  }

  if (mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }

  output = mEnvelope.output * mSequencer.output;
}
