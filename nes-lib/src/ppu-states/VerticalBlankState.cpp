#include "ppu-states/VerticalBlankState.h"
#include "Ppu.h"

VerticalBlankState::VerticalBlankState(Ppu& ppu) : IPpuState(ppu) {}

void VerticalBlankState::Execute() {
  mPpu.mStatusRegister.verticalBlank = 1;
  if (mPpu.mControlRegister.enableNmi) {
    mPpu.nmi = true;
  }

  if ((mPpu.mScanline >= -1 && mPpu.mScanline < 240)) {
    mPpu.Transition<VisibleScreenSpaceState>();
  } else {
    mPpu.Transition<NotVisibleScreenSpaceState>();
  }
}
