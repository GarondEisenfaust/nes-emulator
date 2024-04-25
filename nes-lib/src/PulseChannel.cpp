#include "PulseChannel.h"
#include <Definitions.h>
#include <iostream>

PulseChannel::PulseChannel(bool isSecondChannel)
    : mSweeper(!isSecondChannel), mIsSecondChannel(isSecondChannel), mAddressOffset(mIsSecondChannel * 4) {}

void PulseChannel::Write(uint16_t addr, uint8_t data) {
  if (addr == (0x4000 + mAddressOffset)) {
    switch ((data & 0xC0) >> 6) {
      case 0x00: {
        mPulseWaveGenerator.dutycycle = 0.125;
        break;
      }
      case 0x01: {
        mPulseWaveGenerator.dutycycle = 0.250;
        break;
      }
      case 0x02: {
        mPulseWaveGenerator.dutycycle = 0.500;
        break;
      }
      case 0x03: {
        mPulseWaveGenerator.dutycycle = 0.750;
        break;
      }
    }
    mLengthCounter.SetHalt(data & 0x20);
    mEnvelope.loop = data & 0x20;
    mEnvelope.constantVolume = data & 0x10;
    mEnvelope.volume = data & 0x0F;
  } else if (addr == (0x4001 + mAddressOffset)) {
    mSweeper.Write(data);
  } else if (addr == (0x4002 + mAddressOffset)) {
    mDivider.SetLowerPeriodBits(data);
    UpdateFrequency(mDivider.mPeriod);
  } else if (addr == (0x4003 + mAddressOffset)) {
    mLengthCounter.SetCounter(data >> 3);
    mDivider.SetUpperPeriodBits(data);
    mEnvelope.startFlag = true;
    UpdateFrequency(mDivider.mPeriod);
  } else if (addr == 0x4015) {
    mLengthCounter.SetEnabled(data & (0x01 + mIsSecondChannel));
  }
}

void PulseChannel::Clock(bool quarter, bool half, double globalTime) {
  mDivider.Clock();
  const uint16_t newTimer = mSweeper.Clock(mDivider.mPeriod);

  if (quarter) {
    mEnvelope.Clock();
  }

  if (half) {
    mLengthCounter.Clock();
    if (mSweeper.ShouldSetPeriod()) {
      mDivider.SetPeriod(newTimer);
      UpdateFrequency(mDivider.mPeriod);
    }
  }

  if (mSweeper.ShouldMute() || mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }

  if (mEnvelope.output <= 2 || !mDivider.Notify()) {
    return;
  }

  output = mPulseWaveGenerator.Sample(globalTime, mFrequency, mEnvelope.output - 1);
}

void PulseChannel::UpdateFrequency(uint16_t period) { mFrequency = CPU_CLOCK_SPEED / (16.0 * (period + 1.0)); }
