#pragma once
#include <array>
#include <cstdint>

class LengthCounter {
 public:
  LengthCounter();
  void SetCounter(uint8_t counter);
  void SetHalt(bool halt);
  bool IsHalted();
  bool ShouldMute();
  void SetEnabled(bool enable);
  void Clock();
  uint16_t mCounter;

 private:
  bool mHalt;
  bool mEnabled;
  std::array<int, 32> mLengthTable;
};
