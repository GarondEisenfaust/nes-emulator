#pragma once
#include "rendering/PixelColor.h"
#include <array>

class Pixel {
 public:
  Pixel(float xPos, float yPos, float width, float height);
  Pixel(float xPos, float yPos, float width, float height, PixelColor color);
  void SetColor(PixelColor color);
  PixelColor GetColor();
  std::array<float, 18> CalculateVertexCoordinates();

 private:
  float mXPos;
  float mYPos;
  float mWidth;
  float mHeight;
  PixelColor mColor;
};
