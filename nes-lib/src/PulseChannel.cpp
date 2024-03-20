#include "PulseChannel.h"
#include <iostream>

PulseChannel::PulseChannel(bool isSecondChannel)
    : mSweeper(!isSecondChannel), mIsSecondChannel(isSecondChannel), mAddressOffset(mIsSecondChannel * 4) {}

void PulseChannel::Write(uint16_t addr, uint8_t data) {
  if (addr == (0x4000 + mAddressOffset)) {
    switch ((data & 0xC0) >> 6) {
      case 0x00: {
        mSequencer.reloadSequence = 0b01000000;
        mOscilator.dutycycle = 0.125;
        break;
      }
      case 0x01: {
        mSequencer.reloadSequence = 0b01100000;
        mOscilator.dutycycle = 0.250;
        break;
      }
      case 0x02: {
        mSequencer.reloadSequence = 0b01111000;
        mOscilator.dutycycle = 0.500;
        break;
      }
      case 0x03: {
        mSequencer.reloadSequence = 0b10011111;
        mOscilator.dutycycle = 0.750;
        break;
      }
    }
    mSequencer.Reload();
    mLengthCounter.SetHalt(data & 0x20);
    mEnvelope.loop = data & 0x20;
    mEnvelope.constantVolume = data & 0x10;
    mEnvelope.volume = data & 0x0F;
  } else if (addr == (0x4001 + mAddressOffset)) {
    mSweeper.Write(data);
  } else if (addr == (0x4002 + mAddressOffset)) {
    mSequencer.mDivider.SetLowerPeriodBits(data);
  } else if (addr == (0x4003 + mAddressOffset)) {
    mLengthCounter.SetCounter(data >> 3);
    mSequencer.mDivider.SetUpperPeriodBits(data);
    mEnvelope.startFlag = true;
    mSequencer.Reload();
  } else if (addr == 0x4015) {
    mLengthCounter.SetEnabled(data & (0x01 + mIsSecondChannel));
  }
}

void PulseChannel::Clock(bool quarter, bool half, double globalTime) {
  mSequencer.Clock();
  const uint16_t newTimer = mSweeper.Clock(mSequencer.mDivider.mPeriod);

  if (quarter) {
    mEnvelope.Clock();
  }

  if (half) {
    mLengthCounter.Clock();
    if (mSweeper.ShouldSetPeriod()) {
      mSequencer.mDivider.SetPeriod(newTimer);
    }
  }

  if (mSweeper.ShouldMute() || mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }

  if (mEnvelope.output <= 2 || !mSequencer.mDivider.Notify()) {
    return;
  }

  const double frequency = 1789773.0 / (16.0 * (mSequencer.mDivider.mPeriod + 1.0));
  const double amplitude = mEnvelope.output - 1;
  output = mOscilator.Sample(globalTime, frequency, amplitude);
}
