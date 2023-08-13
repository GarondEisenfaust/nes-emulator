#include "NoiseChannel.h"

NoiseChannel::NoiseChannel() {
  mTimerTable = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};
}

void NoiseChannel::UpdateState(uint16_t addr, uint8_t data) {
  if (addr == 0x400C) {
    mLengthCounter.SetHalt(data & 0x20);
    mEnvelope.loop = data & 0x20;
    mEnvelope.constantVolume = data & 0x10;
    mEnvelope.volume = data & 0x0F;
    return;
  }
  if (addr == 0x400E) {
    mode = data & 8;
    mTimer = mTimerTable[data & 0x0F];
    return;
  }
  if (addr == 0x400F) {
    mLengthCounter.SetCounter((data & 0xF8) >> 3);
    mEnvelope.startFlag = true;
    return;
  }
}

void NoiseChannel::Clock(bool quarter, bool half) {
  if (half) {
    mLengthCounter.Clock();
  }

  if (quarter) {
    mEnvelope.Clock();
  }
  mTimer = std::max(--mTimer, 0);
  auto newBitZero = ShiftRegister();
  if (newBitZero || mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }
  output = mEnvelope.output;
  if (output != 0) {
    auto v = 0;
  }
}

bool NoiseChannel::ShiftRegister() {
  if (mTimer == 0) {
    bool bitZero = shiftRegister & 0x01;
    bool otherBit = shiftRegister & 0x02;
    if (mode) {
      otherBit = shiftRegister & 0x40;
    }

    auto feedBack = bitZero ^ otherBit;
    shiftRegister = (shiftRegister >> 1) | (feedBack << 14);
  }
  return shiftRegister & 0x01;
}
