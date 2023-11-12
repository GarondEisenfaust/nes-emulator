#include "PulseChannel.h"
#include <iostream>

PulseChannel::PulseChannel(bool firstChannel) : mSweeper(firstChannel) {}

void PulseChannel::Clock(bool quarter, bool half) {
  mSequencer.Clock();
  auto newTimer = mSweeper.Clock(mSequencer.mDivider.mPeriod);

  if (quarter) {
    mEnvelope.Clock();
  }

  if (half) {
    mLengthCounter.Clock();
    if (mSweeper.ShouldSetPeriod()) {
      mSequencer.mDivider.SetPeriod(newTimer);
    }
  }

  if (!mSequencer.output) {
    output = 0;
    return;
  }

  if (mSweeper.ShouldMute()) {
    output = 0;
    return;
  }

  if (mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }

  output = mEnvelope.output;
}
