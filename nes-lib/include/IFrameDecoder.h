#pragma once
#include "PixelColor.h"
#include <array>
#include <cstdint>

class IFrameDecoder {
 public:
  virtual void Decode(uint16_t* nesFrameDataBegin, uint16_t* nesFrameDataEnd, unsigned int ppuCycle) = 0;
  virtual PixelColor* GetDecodedFrame() = 0;
};
