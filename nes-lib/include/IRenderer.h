#pragma once
class PixelColor;

class IRenderer {
 public:
  virtual void SetPixelColor(int x, int y, PixelColor& color) = 0;
  virtual void CommitFrame() = 0;
  virtual void StartNewFrame() = 0;
  virtual bool FrameComplete() = 0;
};
