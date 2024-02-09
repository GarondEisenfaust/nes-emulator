#include "LengthCounter.h"
#include <math.h>

LengthCounter::LengthCounter() {
  mLengthTable = {10, 254, 20, 2,  40, 4,  80, 6,  160, 8,  60, 10, 14, 12, 26, 14,
                  12, 16,  24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30};
}

void LengthCounter::SetCounter(uint8_t counter) {
  if (mEnabled) {
    mCounter = mLengthTable[counter];
  }
}

void LengthCounter::SetHalt(bool halt) { mHalt = halt; }

bool LengthCounter::IsHalted() { return mHalt; }

bool LengthCounter::ShouldMute() { return mCounter == 0; }

void LengthCounter::SetEnabled(bool enable) {
  mEnabled = enable;
  if (!mEnabled) {
    mCounter = 0;
  }
}

void LengthCounter::Clock() {
  if (!mHalt && mCounter > 0) {
    mCounter--;
  }
}
