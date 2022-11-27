#pragma once
#include <cstdint>

class Sprite {
 public:
  Sprite(int32_t width, int32_t height);

 private:
  int32_t mWidth;
  int32_t mHeight;
  int32_t mPixels;
};