#pragma once
#include <array>
#include <cstdint>

class IFrameDecoder {
 public:
  virtual void DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle) = 0;
};
