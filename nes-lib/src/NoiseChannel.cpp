#include "NoiseChannel.h"

NoiseChannel::NoiseChannel() {
  mTimerTable = {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};
  mLengthTable = {10, 254, 20, 2,  40, 4,  80, 6,  160, 8,  60, 10, 14, 12, 26, 14,
                  12, 16,  24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30};
}

void NoiseChannel::UpdateState(uint16_t addr, uint8_t data) {
  if (addr == 0x400C) {
    mLengthCounter.SetHalt(!(data & 0x20));
    mEnvelope.loop = data & 0x20;
    mEnvelope.constantVolume = data & 0x10;
    mEnvelope.volume = data & 0x0F;
    return;
  }
  if (addr == 0x400E) {
    mode = data & 0x80;
    mTimer = mTimerTable[data & 0x0F];
    return;
  }
  if (addr == 0x400F) {
    auto index = (data & 0xF8) >> 3;
    mLengthCounter.SetCounter(mLengthTable[index]);
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
  mTimer--;
  mTimer = std::max(mTimer, 0);
  auto newBitZero = ShiftRegister();
  if (newBitZero || mLengthCounter.ShouldMute()) {
    output = 0;
    return;
  }
  output = mEnvelope.output;
}

bool NoiseChannel::ShiftRegister() {
  if (mTimer == 0) {
    bool bitZero = shiftRegister & 0x01;
    bool otherBit = shiftRegister & 0x02;
    if (mode) {
      otherBit = shiftRegister & 0x40;
    }

    auto feedBack = bitZero ^ otherBit;
    auto shiftedByOne = shiftRegister >> 1;
    shiftRegister = (shiftedByOne & ~(1 << 14)) | (feedBack << 14);
  }
  return shiftRegister & 0x01;
}
