#include "PulseChannel.h"
#include <iostream>

PulseChannel::PulseChannel(bool firstChannel) : mSweeper(firstChannel) {}

void PulseChannel::Clock() {
  mEnvelope.Clock();
  auto newTimer = mSweeper.Clock(mSequencer.timer);
  mSequencer.timer = newTimer;
  mSequencer.Clock();

  if (mSweeper.ShouldMute()) {
    output = 0;
    return;
  }
  output = mEnvelope.output * mSequencer.output;
}
