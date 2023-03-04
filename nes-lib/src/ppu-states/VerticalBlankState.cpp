#include "ppu-states/VerticalBlankState.h"
#include "Ppu.h"
#include <cstring>

VerticalBlankState::VerticalBlankState(Ppu& ppu) : IPpuState(ppu) {}

void VerticalBlankState::Execute() {
  if (mPpu.mScanline == 241 && mPpu.mCycle == 1) {
    mPpu.mStatusRegister.verticalBlank = 1;
    if (mPpu.mControlRegister.enableNmi) {
      mPpu.nmi = true;
    }
  }
}
