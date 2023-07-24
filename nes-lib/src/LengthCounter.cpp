#include "LengthCounter.h"
#include <math.h>

void LengthCounter::SetCounter(uint8_t counter) {
  if (mEnabled) {
    mCounter = counter;
  }
}

void LengthCounter::SetHalt(bool halt) { mHalt = halt; }

bool LengthCounter::ShouldMute() { return mCounter == 0; }

void LengthCounter::SetEnabled(bool enable) {
  mEnabled = enable;
  if (!mEnabled) {
    mCounter = 0;
  }
}

void LengthCounter::Clock() {
  if (mHalt && mCounter > 0) {
    mCounter--;
  }
}
