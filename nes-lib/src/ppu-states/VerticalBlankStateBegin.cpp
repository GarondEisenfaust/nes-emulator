#include "ppu-states/VerticalBlankStateBegin.h"
#include "Ppu.h"

VerticalBlankStateBegin::VerticalBlankStateBegin(Ppu& ppu) : IPpuState(ppu) {}

void VerticalBlankStateBegin::Execute() {
  mPpu.mStatusRegister.verticalBlank = 1;
  if (mPpu.mControlRegister.enableNmi) {
    mPpu.nmi = true;
  }

  if ((mPpu.mScanline >= -1 && mPpu.mScanline < 240)) {
    mPpu.Transition<Rendering>();
  } else {
    mPpu.Transition<VerticalBlankState>();
  }
}
