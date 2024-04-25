#include "TriangleChannel.h"
#include "Definitions.h"
#include <iostream>

TriangleChannel::TriangleChannel() { mLengthCounter.SetEnabled(false); }

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
    UpdateFrequency();
  } else if (addr == 0x4015) {
    mLengthCounter.SetEnabled(data & 0x04);
  }
}

void TriangleChannel::Clock(bool quarter, bool half, double globalTime) {
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
  output = mTriangleWaveGenerator.Sample(globalTime, mFrequency);
}

void TriangleChannel::UpdateFrequency() { mFrequency = CPU_CLOCK_SPEED / (32.0 * (mDivider.mPeriod + 1.0)); }
