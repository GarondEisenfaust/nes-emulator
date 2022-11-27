#pragma once
#include "Pixel.h"
#include "RenderContext.h"
#include <vector>

class Grid {
 public:
  Grid(int width, int height);
  void Init();
  Pixel& GetPixel(int x, int y);
  std::vector<float> MakePixelData();
  std::vector<float> MakeColorData();
  void UpdateColor();

 private:
  std::vector<Pixel> mPixels;

  int mWidth;
  int mHeight;
};
