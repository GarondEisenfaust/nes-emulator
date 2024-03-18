#include "Sweeper.h"
#include <algorithm>

Sweeper ::Sweeper(bool onesComplement) : mOnesComplement(onesComplement) {}

uint16_t Sweeper::Clock(uint16_t timerPeriod) {
  auto changeAmount = timerPeriod >> shiftCount;
  if (negate) {
    if (mOnesComplement) {
      changeAmount = -changeAmount;
    } else {
      changeAmount = ~changeAmount;
    }
  }

  auto targetPeriod = std::clamp(timerPeriod + changeAmount, 0, UINT16_MAX);
  mMute = (timerPeriod < 8) || (targetPeriod > 0x07FF);

  if (mDivider.Notify() || mReload) {
    if (!mMute && enable) {
      timerPeriod = targetPeriod;
    }
    mDivider.Reset();
    mReload = false;
  } else {
    mDivider.Clock();
  }
  return timerPeriod;
}

void Sweeper::Write(uint8_t data) {
  enable = data & 0x80;
  mDivider.SetPeriod((data & 0x70) >> 4);
  negate = data & 0x08;
  shiftCount = data & 0x07;
  mReload = true;
}

bool Sweeper::ShouldMute() { return mMute; }

bool Sweeper::ShouldSetPeriod() { return !mMute && shiftCount != 0; }
