#include "Sweeper.h"
#include <algorithm>

Sweeper ::Sweeper(bool onesComplement) : mOnesComplement(onesComplement) {}

uint16_t Sweeper::Clock(uint16_t timerPeriod) {
  mDivider.Clock();
  auto changeAmount = timerPeriod >> shiftCount;
  if (negate) {
    if (mOnesComplement) {
      changeAmount = -changeAmount;
    } else {
      changeAmount = -changeAmount;
    }
  }

  auto targetPeriod = std::clamp(timerPeriod + changeAmount, 0, UINT16_MAX);

  mMute = false;
  if (timerPeriod < 8 || targetPeriod > 0x07FF) {
    mMute = true;
  }
  return mMute ? timerPeriod : targetPeriod;
}

void Sweeper::UpdateState(uint8_t data) {
  enable = data & (1 << 7);
  mDivider.SetPeriod((data & 0x70) >> 4);
  negate = data & (1 << 3);
  shiftCount = (data & 0x07) >> 0;
}

bool Sweeper::ShouldMute() { return mMute; }
