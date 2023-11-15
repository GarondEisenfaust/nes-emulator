#pragma once
#include "IRenderer.h"
#include "Pixel.h"
#include <array>
#include <vector>

class Grid : public IRenderer {
 public:
  Grid(int gridWidth, int gridHeight);

  void SetPixelColor(int x, int y, PixelColor& color);
  void CommitFrame();
  void StartNewFrame();
  bool FrameComplete();

  int GetGridWidth();
  int GetGridHeight();
  std::array<PixelColor, 256 * 240> mTextureData;

 private:
  std::vector<Pixel> mPixels;
  bool mFrameComplete;

  int mGridWidth;
  int mGridHeight;
  int mCurrentPixel = 0;
};
