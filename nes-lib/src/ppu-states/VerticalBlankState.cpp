#include "ppu-states/VerticalBlankState.h"
#include "Ppu.h"
#include <cstring>

VerticalBlankState::VerticalBlankState(Ppu& ppu) : IPpuState(ppu) {}

void VerticalBlankState::Execute() {
  if (mPpu.mScanline == 241 && mPpu.mCycle == 1) {
    mPpu.Transition<VerticalBlankStateBegin>();
  } else if ((mPpu.mScanline >= -1 && mPpu.mScanline < 240)) {
    mPpu.Transition<Rendering>();
  }
}
