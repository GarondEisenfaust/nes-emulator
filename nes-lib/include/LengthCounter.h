#pragma once
#include <cstdint>

class LengthCounter {
 public:
  void SetCounter(uint8_t counter);
  void SetHalt(bool halt);
  bool ShouldMute();
  void SetEnabled(bool enable);
  void Clock();
  uint8_t mCounter;

 private:
  bool mHalt;
  bool mEnabled;
};
