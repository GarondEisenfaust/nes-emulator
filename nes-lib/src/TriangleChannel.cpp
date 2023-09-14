#include "TriangleChannel.h"
#include <iostream>

TriangleChannel::TriangleChannel() {
  mSequence = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,  4,  3,  2,  1,  0,
               0,  1,  2,  3,  4,  5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  mLengthCounter.SetEnabled(true);
}

TriangleChannel::~TriangleChannel() {}

void TriangleChannel::UpdateState(uint16_t addr, uint8_t data) {
  if (addr == 0x4008) {
    mCounterControlFlag = data & 0x80;
    mCounterReloadValue = data & 0x7F;
    mLengthCounter.SetHalt(data & 0x80);
  } else if (addr == 0x400A) {
    mDivider.SetPeriod((mDivider.mPeriod & 0x0700) | data);
  } else if (addr == 0x400B) {
    mLengthCounter.SetCounter(data >> 3);
    mDivider.SetPeriod((mDivider.mPeriod & 0x00FF) | ((data & 0x07) << 7));
    mCounterReload = true;
  }
}

void TriangleChannel::Clock(bool quarter, bool half) {
  if (!(quarter || half)) {
    return;
  }

  mDivider.Clock();
  if (!mDivider.Notify()) {
    return;
  }

  if (!mCounterControlFlag) {
    mCounterReload = false;
  }
  if (mCounterReload) {
    mCounter = mCounterReloadValue;
  } else if (mCounter > 0) {
    mCounter--;
  }

  output = 0;
  mLengthCounter.Clock();
  std::cout << std::to_string(mLengthCounter.mCounter) << "\n";

  if (mLengthCounter.ShouldMute()) {
    return;
  }

  output = mSequence[mSequenceIndex];
  std::cout << std::to_string(output) << "\n";
  if (mCounter > 0) {
    mSequenceIndex++;
  }
}
