#include "Divider.h"

void Divider::Clock() {
  // if (mReload) {
  //   mCounter = mPeriod;
  //   mReload = false;
  //   return;
  // }
  mCounter--;
  mNotify = false;
  if (mCounter > 0 || mReload) {
    mReload = false;
    return;
  }
  Reset();
  // mCounter = mPeriod;
  mNotify = true;
}

void Divider::SetPeriod(int period) { mPeriod = period; }

// void Divider::Reset() { mReload = true; }
void Divider::Reset() {
  mCounter = mPeriod;
  // mReload = true;
}

bool Divider::Notify() { return mNotify; }
