#include "Apu.h"
#include "Bus.h"

void Apu::CpuWrite(uint16_t addr, uint8_t data) {
  if (addr == 0x4000) {
    switch ((data & 0xC0) >> 6) {
      case 0x00:
        // mPulseSequencerOne.sequence = 0b01000000;
        mPulseOscilatorOne.dutycycle = 0.125;
        break;
      case 0x01:
        // mPulseSequencerOne.sequence = 0b01100000;
        mPulseOscilatorOne.dutycycle = 0.250;
        break;
      case 0x02:
        // mPulseSequencerOne.sequence = 0b01111000;
        mPulseOscilatorOne.dutycycle = 0.500;
        break;
      case 0x03:
        // mPulseSequencerOne.sequence = 0b10011111;
        mPulseOscilatorOne.dutycycle = 0.750;
        break;
    }
    mPulseSequencerOne.enabled = (data & 0x20);
  } else if (addr == 0x4002) {
    mPulseSequencerOne.reload = (mPulseSequencerOne.sequence & 0xFF00) | data;
  } else if (addr == 0x4003) {
    mPulseSequencerOne.reload = static_cast<uint16_t>(data & 0x07) << 8 | (mPulseSequencerOne.reload & 0x00FF);
    mPulseSequencerOne.timer = mPulseSequencerOne.reload;
  } else if (addr == 0x4004) {
    switch ((data & 0xC0) >> 6) {
      case 0x00:
        // mPulseSequencerTwo.sequence = 0b01000000;
        mPulseOscilatorTwo.dutycycle = 0.125;
        break;
      case 0x01:
        // mPulseSequencerTwo.sequence = 0b01100000;
        mPulseOscilatorTwo.dutycycle = 0.250;
        break;
      case 0x02:
        // mPulseSequencerTwo.sequence = 0b01111000;
        mPulseOscilatorTwo.dutycycle = 0.500;
        break;
      case 0x03:
        // mPulseSequencerTwo.sequence = 0b10011111;
        mPulseOscilatorTwo.dutycycle = 0.750;
        break;
    }
    mPulseSequencerTwo.enabled = (data & 0x20);
  } else if (addr == 0x4005) {
    mPulseSequencerTwo.reload = (mPulseSequencerTwo.sequence & 0xFF00) | data;
  } else if (addr == 0x4006) {
    mPulseSequencerTwo.reload = static_cast<uint16_t>(data & 0x07) << 8 | (mPulseSequencerTwo.reload & 0x00FF);
    mPulseSequencerTwo.timer = mPulseSequencerTwo.reload;
  } else if (addr == 0x4015) {
    mPulseSequencerTwo.enabled = data & 0x01;
    mPulseSequencerTwo.enabled = data & 0x02;
  }
}

uint8_t Apu::CpuRead(uint16_t addr) {
  if (addr == 0x4015) {
    auto pulseTimerOne = mPulseSequencerOne.timer != 0;
    auto pulseTimerTwo = mPulseSequencerTwo.timer != 0;
    return (pulseTimerTwo << 1) | pulseTimerOne;
  }
  return 0;
}

float Apu::GetNextSample() {
  auto result = buffer.Read();
  return result;
}

void Apu::Clock() {
  if (mClockCounter % 6 == 0) {
    mPulseSequencerOne.Clock();
    mPulseSequencerTwo.Clock();
    auto output = mPulseSequencerOne.output + mPulseSequencerTwo.output;
    buffer.Write(output);
  }
  mClockCounter++;
}

void Apu::Reset() {}

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}
