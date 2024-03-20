#pragma once
#include <cstdint>

class Divider {
 public:
  void Clock();
  void SetPeriod(uint16_t period);
  void SetLowerPeriodBits(uint8_t period);
  void SetUpperPeriodBits(uint8_t period);
  void Reset();
  void Reset(uint16_t period);
  bool Notify();
  uint16_t mPeriod = 0;
  long mCounter = 0;

 private:
  bool mReload;
  bool mNotify = false;
};
