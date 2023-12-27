#pragma once
#include "PixelColor.h"
#include <cstdint>

class PixelColor;

class IRenderer {
 public:
  virtual void SetPixelColor(int x, int y, PixelColor& color) = 0;
  virtual void CommitFrame() = 0;
  virtual void StartNewFrame() = 0;
  virtual bool FrameComplete() = 0;
  virtual void SetData(int index, PixelColor color) = 0;
};
