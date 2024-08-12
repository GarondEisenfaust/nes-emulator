#pragma once
#include "PixelColor.h"
#include <array>

class Pixel {
 public:
  Pixel(float xPos, float yPos, float width, float height);
  Pixel(float xPos, float yPos, float width, float height, PixelColorU8 color);
  void SetColor(PixelColorU8 color);
  PixelColorU8 GetColor();
  std::array<float, 18> CalculateVertexCoordinates();

 private:
  float mXPos;
  float mYPos;
  float mWidth;
  float mHeight;
  PixelColorU8 mColor;
};
