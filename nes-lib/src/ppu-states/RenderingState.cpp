#include "ppu-states/RenderingState.h"
#include "Ppu.h"
#include <cstring>

RenderingState::RenderingState(Ppu& ppu) : IPpuState(ppu) {}

void RenderingState::Execute() {
  if (mPpu.mScanline == 0 && mPpu.mCycle == 0) {
    mPpu.mCycle = 1;
  }

  if (mPpu.mScanline == -1 && mPpu.mCycle == 1) {
    mPpu.mStatusRegister.verticalBlank = false;
    mPpu.mStatusRegister.spriteOverflow = false;
    mPpu.mStatusRegister.spriteZeroHit = false;
    mPpu.mSpriteShifterPattern.low = {0};
    mPpu.mSpriteShifterPattern.high = {0};
  }

  if (mPpu.mCycle >= 2) {
    mPpu.VRamFetch();
  }
}
