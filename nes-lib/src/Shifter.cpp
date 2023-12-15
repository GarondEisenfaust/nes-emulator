#include "Shifter.h"

void Shifter::Shift() {
  mLower <<= 1;
  mUpper <<= 1;
}

uint8_t Shifter::ReadFrom(unsigned int index) {
  bool lowBit = mLower & index;
  bool highBit = mUpper & index;
  return (highBit << 1) | lowBit;
}

void Shifter::Load(uint8_t upper, uint8_t lower) {
  mLower = (mLower & 0xFF00) | lower;
  mUpper = (mUpper & 0xFF00) | upper;
}

void Shifter::Reset() {
  mLower = 0;
  mUpper = 0;
}
