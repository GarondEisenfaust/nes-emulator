#include "TriangleChannel.h"
#include <iostream>

TriangleChannel::TriangleChannel() {
  mSequence = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,  4,  3,  2,  1,  0,
               0,  1,  2,  3,  4,  5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  mLengthCounter.SetEnabled(true);
}

TriangleChannel::~TriangleChannel() {}

void TriangleChannel::Write(uint16_t addr, uint8_t data) {
  if (addr == 0x4008) {
    mCounterControlFlag = data & 0x80;
    mLengthCounter.SetHalt(data & 0x80);
    mCounterReloadValue = data & 0x7F;
  } else if (addr == 0x400A) {
    mDivider.SetLowerPeriodBits(data);
  } else if (addr == 0x400B) {
    mLengthCounter.SetCounter(data >> 3);
    mDivider.SetUpperPeriodBits(data);
    mCounterReload = true;
  } else if (addr == 0x4015) {
    mLengthCounter.SetEnabled(data & 0x04);
  }
}

void TriangleChannel::Clock(bool quarter, bool half) {
  if (quarter || half) {
    if (mCounterReload) {
      mCounter = mCounterReloadValue;
    } else if (mCounter > 0) {
      mCounter--;
    }
    if (!mCounterControlFlag) {
      mCounterReload = false;
    }
  }

  if (half) {
    mLengthCounter.Clock();
  }

  mDivider.Clock();
  if (!mDivider.Notify()) {
    return;
  }

  if (mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }

  if (mCounter <= 0) {
    return;
  }
  output = mSequence[mSequenceIndex];
  mSequenceIndex = (mSequenceIndex + 1) % mSequence.size();
}
