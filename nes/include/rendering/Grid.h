#pragma once
#include "IRenderer.h"
#include "Pixel.h"
#include <array>
#include <vector>

class Grid : public IRenderer {
 public:
  Grid(int width, int height, int gridWidth, int gridHeight);
  void Init();

  void SetPixelColor(int x, int y, PixelColor& color);
  void CommitFrame();
  void StartNewFrame();
  bool FrameComplete();

  Pixel& GetPixel(int x, int y);
  std::vector<float> MakePixelData();
  std::vector<float> MakeColorData();

  int GetGridWidth();
  int GetGridHeight();
  std::array<PixelColor, 256 * 240> mTextureData;

 private:
  std::vector<Pixel> mPixels;
  bool mFrameComplete;
  int mWidth;
  int mHeight;

  int mGridWidth;
  int mGridHeight;
  int mCurrentPixel = 0;
};
