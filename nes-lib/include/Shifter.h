#pragma once
#include <cstdint>

class Shifter {
 public:
  void Shift();
  uint8_t ReadFrom(unsigned int index = 0);
  void Load(uint8_t upper, uint8_t lower);
  void Reset();

 private:
  uint16_t mLower;
  uint16_t mUpper;
};
