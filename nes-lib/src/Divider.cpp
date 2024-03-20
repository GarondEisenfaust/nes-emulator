#include "Divider.h"

void Divider::Clock() {
  mNotify = false;
  if (mReload) {
    mReload = false;
    mCounter = mPeriod;
    return;
  }
  mCounter--;

  if (mCounter <= 0) {
    Reset();
    mNotify = true;
  }
}

void Divider::SetPeriod(uint16_t period) { mPeriod = period; }

void Divider::SetLowerPeriodBits(uint8_t period) { SetPeriod((mPeriod & 0xFF00) | period); }

void Divider::SetUpperPeriodBits(uint8_t period) { SetPeriod((period & 0x07) << 8 | (mPeriod & 0x00FF)); }

void Divider::Reset() { mReload = true; }

void Divider::Reset(uint16_t period) {
  SetPeriod(period);
  Reset();
}

bool Divider::Notify() { return mNotify; }
