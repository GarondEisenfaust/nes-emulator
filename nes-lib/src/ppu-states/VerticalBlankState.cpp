#include "ppu-states/VerticalBlankState.h"
#include "Bus.h"
#include "Ppu.h"
#include <cstring>

VerticalBlankState::VerticalBlankState(Ppu& ppu) : IPpuState(ppu) {}

void VerticalBlankState::Execute() {
  if (mPpu.mScanline != 241 || mPpu.mCycle != 1) {
    return;
  }
  mPpu.mStatusRegister.verticalBlank = true;
  if (mPpu.mControlRegister.enableNmi) {
    mPpu.NonMaskableInterrupt();
  }
}
