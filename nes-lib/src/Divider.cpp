#include "Divider.h"

void Divider::Clock() {
  mCounter--;
  mNotify = false;
  if (mCounter > 0) {
    return;
  }
  Reset();
  mNotify = true;
}

void Divider::SetPeriod(int period) { mPeriod = period; }

void Divider::Reset() { mCounter = mPeriod; }

bool Divider::Notify() { return mNotify; }
