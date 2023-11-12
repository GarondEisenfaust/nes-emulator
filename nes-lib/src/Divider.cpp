#include "Divider.h"

void Divider::Clock() {
  mNotify = false;
  if (mReload) {
    mReload = false;
    return;
  }
  mCounter--;

  if (mCounter <= 0) {
    Reset();
    mNotify = true;
  }
}

void Divider::SetPeriod(int period) { mPeriod = period; }

void Divider::SetLowerPeriodBits(int period) { SetPeriod((mPeriod & 0xFF00) | period); }

void Divider::SetUpperPeriodBits(int period) { SetPeriod((period & 0x07) << 8 | (mPeriod & 0x00FF)); }

void Divider::Reset() {
  mCounter = mPeriod;
  mReload = true;
}

bool Divider::Notify() { return mNotify; }
