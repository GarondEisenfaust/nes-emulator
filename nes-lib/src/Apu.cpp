#include "Apu.h"
#include "Bus.h"

void Apu::CpuWrite(uint16_t addr, uint8_t data) {
  if (addr == 0x4000) {
    switch ((data & 0xC0) >> 6) {
      case 0x00:
        mPulseChannelOne.mSequencer.sequence = 0b01000000;
        mPulseChannelOne.mOscilator.dutycycle = 0.125;
        break;
      case 0x01:
        mPulseChannelOne.mSequencer.sequence = 0b01100000;
        mPulseChannelOne.mOscilator.dutycycle = 0.250;
        break;
      case 0x02:
        mPulseChannelOne.mSequencer.sequence = 0b01111000;
        mPulseChannelOne.mOscilator.dutycycle = 0.500;
        break;
      case 0x03:
        mPulseChannelOne.mSequencer.sequence = 0b10011111;
        mPulseChannelOne.mOscilator.dutycycle = 0.750;
        break;
    }
    mPulseChannelOne.mSequencer.enabled = data & 0x20;
    mPulseChannelOne.mEnvelope.startFlag = data & 0x10;
    mPulseChannelOne.mEnvelope.volume = data & 0x0F;
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
        mPulseChannelTwo.mOscilator.dutycycle = 0.125;
        break;
      case 0x01:
        mPulseChannelTwo.mSequencer.sequence = 0b01100000;
        mPulseChannelTwo.mOscilator.dutycycle = 0.250;
        break;
      case 0x02:
        mPulseChannelTwo.mSequencer.sequence = 0b01111000;
        mPulseChannelTwo.mOscilator.dutycycle = 0.500;
        break;
      case 0x03:
        mPulseChannelTwo.mSequencer.sequence = 0b10011111;
        mPulseChannelTwo.mOscilator.dutycycle = 0.750;
        break;
    }
    mPulseChannelTwo.mSequencer.enabled = data & 0x20;
    mPulseChannelTwo.mEnvelope.startFlag = data & 0x10;
    mPulseChannelTwo.mEnvelope.volume = data & 0x0F;
  } else if (addr == 0x4005) {
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

float Apu::GetNextSample() {
  if (queue.size() == 0) {
    return 0;
  }

  auto result = queue.front();
  queue.pop();
  return result;
}

void Apu::Clock() {
  if (mClockCounter % 6 == 0) {
    mPulseChannelOne.Clock();
    mPulseChannelTwo.Clock();
    mPulseChannelOne.mOscilator.frequency =
        1789773.0 / (16.0 * static_cast<double>(mPulseChannelOne.mSequencer.reload + 1));
    mPulseChannelTwo.mOscilator.frequency =
        1789773.0 / (16.0 * static_cast<double>(mPulseChannelTwo.mSequencer.reload + 1));

    auto output = mPulseChannelOne.mSequencer.output + mPulseChannelTwo.mSequencer.output;
    queue.push(output);
  }
  mClockCounter++;
}

void Apu::Reset() {}

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}
