#include "Apu.h"
#include "Bus.h"

Apu::Apu(IAudioOutputDevice& outputDevice)
    : mOutputDevice(outputDevice), mPulseChannelOne(true), mPulseChannelTwo(false) {}

void Apu::CpuWrite(uint16_t addr, uint8_t data) {
  if (addr == 0x4000) {
    switch ((data & 0xC0) >> 6) {
      case 0x00:
        mPulseChannelOne.mSequencer.sequence = 0b01000000;
        break;
      case 0x01:
        mPulseChannelOne.mSequencer.sequence = 0b01100000;
        break;
      case 0x02:
        mPulseChannelOne.mSequencer.sequence = 0b01111000;
        break;
      case 0x03:
        mPulseChannelOne.mSequencer.sequence = 0b10011111;
        break;
    }
    // mPulseChannelOne.mSequencer.enabled = data & 0x20;
    mPulseChannelOne.mEnvelope.startFlag = data & 0x10;
    mPulseChannelOne.mEnvelope.volume = data & 0x0F;
  } else if (addr == 0x4001) {
    mPulseChannelOne.mSweeper.UpdateState(data);
  } else if (addr == 0x4002) {
    mPulseChannelOne.mSequencer.reload = (mPulseChannelOne.mSequencer.sequence & 0xFF00) | data;
  } else if (addr == 0x4003) {
    mPulseChannelOne.mSequencer.reload =
        static_cast<uint16_t>(data & 0x07) << 8 | (mPulseChannelOne.mSequencer.reload & 0x00FF);
    mPulseChannelOne.mSequencer.timer = mPulseChannelOne.mSequencer.reload;
  } else if (addr == 0x4004) {
    switch ((data & 0xC0) >> 6) {
      case 0x00:
        mPulseChannelTwo.mSequencer.sequence = 0b01000000;
        break;
      case 0x01:
        mPulseChannelTwo.mSequencer.sequence = 0b01100000;
        break;
      case 0x02:
        mPulseChannelTwo.mSequencer.sequence = 0b01111000;
        break;
      case 0x03:
        mPulseChannelTwo.mSequencer.sequence = 0b10011111;
        break;
    }
    // mPulseChannelTwo.mSequencer.enabled = data & 0x20;
    mPulseChannelTwo.mEnvelope.startFlag = data & 0x10;
    mPulseChannelTwo.mEnvelope.volume = data & 0x0F;
  } else if (addr == 0x4005) {
    mPulseChannelTwo.mSweeper.UpdateState(data);
    mPulseChannelTwo.mSequencer.reload = (mPulseChannelTwo.mSequencer.sequence & 0xFF00) | data;
  } else if (addr == 0x4006) {
    mPulseChannelTwo.mSequencer.reload =
        static_cast<uint16_t>(data & 0x07) << 8 | (mPulseChannelTwo.mSequencer.reload & 0x00FF);
    mPulseChannelTwo.mSequencer.timer = mPulseChannelTwo.mSequencer.reload;
  } else if (addr == 0x4015) {
    mPulseChannelTwo.mSequencer.enabled = data & 0x01;
    mPulseChannelTwo.mSequencer.enabled = data & 0x02;
  }
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
    mPulseChannelOne.Clock();
    mPulseChannelTwo.Clock();
    output = static_cast<float>(mPulseChannelOne.output + mPulseChannelTwo.output);
  }

  if (mClockCounter % 121 == 0) {
    mOutputDevice.Write(output);
  }
  mClockCounter++;
}

void Apu::Reset() {}

void Apu::Lock() { queueMutex.lock(); }

void Apu::Unlock() { queueMutex.unlock(); }

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}
