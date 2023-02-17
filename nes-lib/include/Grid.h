#pragma once
#include "Pixel.h"
#include <vector>

class Grid {
 public:
  Grid(int width, int height, int gridWidth, int gridHeight);
  void Init();
  Pixel& GetPixel(int x, int y);
  std::vector<float> MakePixelData();
  std::vector<float> MakeColorData();
  void UpdateColor();
  int GetGridWidth();
  int GetGridHeight();

 private:
  std::vector<Pixel> mPixels;

  int mWidth;
  int mHeight;

  int mGridWidth;
  int mGridHeight;
};
