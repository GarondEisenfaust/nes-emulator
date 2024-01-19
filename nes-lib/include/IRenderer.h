#pragma once
#include "PixelColor.h"
#include <cstdint>

class PixelColor;

class IRenderer {
 public:
  virtual void SetNesPixel(int x, int y, uint8_t pixel) = 0;
  virtual void CommitFrame(unsigned int ppuCycle) = 0;
  virtual void StartNewFrame() = 0;
  virtual bool FrameComplete() = 0;
};
