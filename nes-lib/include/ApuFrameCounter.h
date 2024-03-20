#pragma once
#include <cstdint>

#define APU_FRAME_COUNTER 0x4017

class Apu;
class ApuFrameCounter {
 public:
  ApuFrameCounter(Apu& apu);
  void Write(uint16_t addr, uint8_t data);
  void Clock();
  bool mHalfFrameClock = false;
  bool mQuarterFrameClock = false;

 private:
  bool mInterruptInhibit = false;
  bool mFiveStepMode = false;
  uint32_t mFrameCounter = 0;
  Apu& mApu;
  const uint32_t mFourStepHalfFrameClocks[2] = {7456, 14914};
  const uint32_t mFourStepQuarterFrameClocks[4] = {3728, mFourStepHalfFrameClocks[0], 11185,
                                                   mFourStepHalfFrameClocks[1]};
  const uint32_t mFiveStepHalfFrameClocks[2] = {7456, 18640};
  const uint32_t mFiveStepQuarterFrameClocks[4] = {3728, mFiveStepHalfFrameClocks[0], 11185,
                                                   mFiveStepHalfFrameClocks[1]};

  bool IsHalfFrameClock(uint32_t clock);
  bool IsQuarterFrameClock(uint32_t clock);
  bool HasToSetInterrupt();
};
