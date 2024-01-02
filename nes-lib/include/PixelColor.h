#pragma once
#include <cstdint>
#include <functional>

struct PixelColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  bool operator==(const PixelColor& other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
  }
};

template <>
struct std::hash<PixelColor> {
  std::size_t operator()(const PixelColor& k) const {
    return ((std::hash<uint8_t>()(k.r) ^ (std::hash<uint8_t>()(k.g) << 1)) >> 1) ^ (std::hash<uint8_t>()(k.b) << 1) ^
           (std::hash<uint8_t>()(k.a) << 2);
  }
};
