#pragma once
#include <cstdint>

struct PixelInfo {
  uint8_t pixel = 0;
  uint8_t palette = 0;
};

struct BackgroundPixelInfo {
  uint8_t pixel = 0;
  uint8_t palette = 0;
};

struct ForegroundPixelInfo {
  uint8_t pixel = 0;
  uint8_t palette = 0;
  bool priority = 0;
};
