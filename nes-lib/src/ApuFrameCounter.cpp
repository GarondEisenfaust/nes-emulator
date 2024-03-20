#include "ApuFrameCounter.h"
#include "Apu.h"
#include <iostream>

ApuFrameCounter::ApuFrameCounter(Apu& apu) : mApu(apu) {}

void ApuFrameCounter::Write(uint16_t addr, uint8_t data) {
  if (addr == APU_FRAME_COUNTER) {
    mFiveStepMode = data & 0x80;
    mInterruptInhibit = data & 0x40;

    if (mFiveStepMode) {
      mQuarterFrameClock = true;
      mHalfFrameClock = true;
    }
    mFrameCounter = 0;

    if (mInterruptInhibit) {
      mApu.mInterrupt = false;
    }
  }
}

template <typename T, typename... Opts>
inline bool IsAnyOf(T val, Opts... opts) {
  return (... || (val == opts));
}

bool ApuFrameCounter::IsHalfFrameClock(uint32_t clock) {
  if (mFiveStepMode) {
    return IsAnyOf(clock, mFiveStepHalfFrameClocks[0], mFiveStepHalfFrameClocks[1]);
  }
  return IsAnyOf(clock, mFourStepHalfFrameClocks[0], mFourStepHalfFrameClocks[1]);
}

bool ApuFrameCounter::IsQuarterFrameClock(uint32_t clock) {
  if (mFiveStepMode) {
    return IsAnyOf(clock, mFiveStepQuarterFrameClocks[0], mFiveStepQuarterFrameClocks[1],
                   mFiveStepQuarterFrameClocks[2], mFiveStepQuarterFrameClocks[3]);
  }
  return IsAnyOf(clock, mFourStepQuarterFrameClocks[0], mFourStepQuarterFrameClocks[1], mFourStepQuarterFrameClocks[2],
                 mFourStepQuarterFrameClocks[3]);
}

void ApuFrameCounter::Clock() {
  mHalfFrameClock = IsHalfFrameClock(mFrameCounter);
  mQuarterFrameClock = IsQuarterFrameClock(mFrameCounter);
  if (HasToSetInterrupt()) {
    mApu.mInterrupt = true;
  }

  mFrameCounter = (mFrameCounter + 1) % (mFiveStepMode ? mFiveStepHalfFrameClocks[1] : mFourStepHalfFrameClocks[1]);
}

bool ApuFrameCounter::HasToSetInterrupt() {
  return !mInterruptInhibit && !mFiveStepMode && mFrameCounter == mFourStepHalfFrameClocks[1];
}
