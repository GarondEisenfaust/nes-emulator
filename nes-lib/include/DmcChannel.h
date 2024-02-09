#pragma once
#include "Divider.h"
#include <array>
#include <cstdint>

class Apu;
class DmcChannel {
 public:
  DmcChannel(Apu* apu);
  ~DmcChannel();
  void Write(uint16_t addr, uint8_t data);
  void Clock();
  uint8_t ReadSample(uint16_t addr);
  void Restart();
  void IncrementCurrentSampleAddress();
  void Interrupt();

  Apu* mApu;
  bool mInteruptEnabled = false;
  bool mInterupt;
  bool mLoop;
  bool mSilence;
  uint8_t output = 0;
  uint8_t mShiftRegisterRemaining = 0;
  uint8_t mSampleBuffer = 0x00;
  uint16_t mCurrentSampleAddress = 0x0000;
  uint16_t mBytesRemaining = 0;
  uint16_t mSampleAddress = 0x0000;
  uint16_t mSampleLength = 0;
  uint8_t mShiftRegister = 0;
  std::array<uint16_t, 16> mRateTable;
  Divider mDivider;
};
