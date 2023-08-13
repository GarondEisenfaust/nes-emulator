#include "Apu.h"
#include "Bus.h"
#include <algorithm>
#include <array>
#include <math.h>

Apu::Apu(IAudioOutputDevice& outputDevice)
    : mOutputDevice(outputDevice), mPulseChannelOne(true), mPulseChannelTwo(false) {}

void Apu::CpuWrite(uint16_t addr, uint8_t data) {
  if (addr == 0x4000) {
    switch ((data & 0xC0) >> 6) {
      case 0x00: {
        mPulseChannelOne.mSequencer.sequence = 0b01000000;
        break;
      }
      case 0x01: {
        mPulseChannelOne.mSequencer.sequence = 0b01100000;
        break;
      }
      case 0x02: {
        mPulseChannelOne.mSequencer.sequence = 0b01111000;
        break;
      }
      case 0x03: {
        mPulseChannelOne.mSequencer.sequence = 0b10011111;
        break;
      }
    }
    mPulseChannelOne.mLengthCounter.SetHalt(data & 0x20);
    // mPulseChannelOne.mEnvelope.loop = data & 0x20;
    mPulseChannelOne.mEnvelope.constantVolume = data & 0x10;
    mPulseChannelOne.mEnvelope.volume = data & 0x0F;
  } else if (addr == 0x4001) {
    mPulseChannelOne.mSweeper.UpdateState(data);
  } else if (addr == 0x4002) {
    mPulseChannelOne.mSequencer.reload = (mPulseChannelOne.mSequencer.sequence & 0xFF00) | data;
  } else if (addr == 0x4003) {
    mPulseChannelOne.mLengthCounter.SetCounter((data & 0xF8) >> 3);
    mPulseChannelOne.mSequencer.reload =
        static_cast<uint16_t>(data & 0x07) << 8 | (mPulseChannelOne.mSequencer.reload & 0x00FF);
    mPulseChannelOne.mSequencer.timer = mPulseChannelOne.mSequencer.reload;
    mPulseChannelOne.mEnvelope.startFlag = true;
  } else if (addr == 0x4004) {
    switch ((data & 0xC0) >> 6) {
      case 0x00: {
        mPulseChannelTwo.mSequencer.sequence = 0b01000000;
        break;
      }
      case 0x01: {
        mPulseChannelTwo.mSequencer.sequence = 0b01100000;
        break;
      }
      case 0x02: {
        mPulseChannelTwo.mSequencer.sequence = 0b01111000;
        break;
      }
      case 0x03: {
        mPulseChannelTwo.mSequencer.sequence = 0b10011111;
        break;
      }
    }
    mPulseChannelTwo.mLengthCounter.SetHalt(data & 0x20);
    // mPulseChannelTwo.mEnvelope.loop = data & 0x20;
    mPulseChannelTwo.mEnvelope.constantVolume = data & 0x10;
    mPulseChannelTwo.mEnvelope.volume = data & 0x0F;
  } else if (addr == 0x4005) {
    mPulseChannelTwo.mSweeper.UpdateState(data);
  } else if (addr == 0x4006) {
    mPulseChannelTwo.mSequencer.reload = (mPulseChannelTwo.mSequencer.sequence & 0xFF00) | data;
  } else if (addr == 0x4007) {
    mPulseChannelTwo.mLengthCounter.SetCounter((data & 0xF8) >> 3);
    mPulseChannelTwo.mSequencer.reload =
        static_cast<uint16_t>(data & 0x07) << 8 | (mPulseChannelTwo.mSequencer.reload & 0x00FF);
    mPulseChannelTwo.mSequencer.timer = mPulseChannelTwo.mSequencer.reload;
    mPulseChannelTwo.mEnvelope.startFlag = true;
  } else if (addr == 0x400F) {
    mPulseChannelOne.mEnvelope.startFlag = true;
    mPulseChannelTwo.mEnvelope.startFlag = true;
  } else if (addr == 0x4015) {
    mPulseChannelOne.mSequencer.enabled = data & 0x01;
    mPulseChannelTwo.mSequencer.enabled = data & 0x02;
    mPulseChannelOne.mLengthCounter.SetEnabled(data & 0x01);
    mPulseChannelTwo.mLengthCounter.SetEnabled(data & 0x02);
  }
  mNoiseChannel.UpdateState(addr, data);
}

uint8_t Apu::CpuRead(uint16_t addr) {
  if (addr == 0x4015) {
    auto pulseTimerOne = mPulseChannelOne.mSequencer.timer != 0;
    auto pulseTimerTwo = mPulseChannelTwo.mSequencer.timer != 0;
    return (pulseTimerTwo << 1) | pulseTimerOne;
  }
  return 0;
}

void Apu::Clock() {
  if (mClockCounter % 6 == 0) {
    auto quarter = IsQuarterFrameClock(mFrameClockCounter);
    auto half = IsHalfFrameClock(mFrameClockCounter);

    mPulseChannelOne.Clock(quarter, half);
    mPulseChannelTwo.Clock(quarter, half);
    mNoiseChannel.Clock(quarter, half);
    output = Mix(mPulseChannelOne.output, mPulseChannelTwo.output, mNoiseChannel.output);

    if (mFrameClockCounter % 20 == 0) {
      mOutputDevice.Write(output);
    }
    mFrameClockCounter = (mFrameClockCounter + 1) % 14833;
  }
  mClockCounter++;
}

void Apu::Reset() {}

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}

template <typename T, typename... Opts>
inline bool IsAnyOf(T val, Opts... opts) {
  return (... || (val == opts));
}

bool Apu::IsHalfFrameClock(int clock) { return IsAnyOf(clock, halfFrameClocks[0], halfFrameClocks[1]); }

bool Apu::IsQuarterFrameClock(int clock) {
  return IsAnyOf(clock, quarterFrameClocks[0], quarterFrameClocks[1], quarterFrameClocks[2], quarterFrameClocks[3]);
}

inline float Apu::Mix(uint8_t pulseOneOutput, uint8_t pulseTwoOutput, uint8_t noiseOutput) {
  auto pulseOut = 0.00752f * static_cast<float>(mPulseChannelOne.output + mPulseChannelTwo.output);
  auto tndOut = 0.00494 * noiseOutput;
  return pulseOut + tndOut;
}
