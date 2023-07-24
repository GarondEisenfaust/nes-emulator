#pragma once
#include <cstdint>

class LengthCounter {
 public:
  void SetCounter(uint8_t counter);
  void SetHalt(bool halt);
  bool ShouldMute();
  void SetEnabled(bool enable);
  void Clock();

 private:
  uint8_t mCounter;
  bool mHalt;
  bool mEnabled;
};
