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
  uint8_t ReadSample();
  void Restart();
  void IncrementCurrentSampleAddress();

  Apu* mApu;
  bool mInterruptEnabled = false;
  bool mLoop;
  bool mSilence;
  bool mStart = false;
  uint8_t output = 0;
  uint8_t mBitsRemaining = 0;
  uint8_t mSampleBuffer = 0x00;
  bool mSampleBufferEmpty = true;
  uint16_t mCurrentSampleAddress = 0x0000;
  uint16_t mBytesRemaining = 0;
  uint16_t mSampleAddress = 0x0000;
  uint16_t mSampleLength = 0;
  uint8_t mShiftRegister = 0;
  std::array<uint16_t, 16> mRateTable;
  Divider mDivider;
};
