#include "ppu-states/NotVisibleScreenSpaceState.h"
#include "PixelProcessingUnit.h"
#include <cstring>

NotVisibleScreenSpaceState::NotVisibleScreenSpaceState(PixelProcessingUnit& ppu) : IPpuState(ppu) {}

void NotVisibleScreenSpaceState::Execute() {
  if (mPpu.mScanline == 241 && mPpu.mCycle == 1) {
    mPpu.Transition<VerticalBlankState>();
  } else if ((mPpu.mScanline >= -1 && mPpu.mScanline < 240)) {
    mPpu.Transition<VisibleScreenSpaceState>();
  }
}
