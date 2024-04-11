#pragma once
#include <cstdint>
#include <functional>

template <typename T>
struct PixelColor {
  T r;
  T g;
  T b;
  T a;

  bool operator==(const PixelColor& other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
  }
};

using PixelColorU8 = PixelColor<uint8_t>;
using PixelColorF = PixelColor<float>;

template <typename T>
struct std::hash<PixelColor<T>> {
  std::size_t operator()(const PixelColor<T>& k) const {
    return ((std::hash<T>()(k.r) ^ (std::hash<T>()(k.g) << 1)) >> 1) ^ (std::hash<T>()(k.b) << 1) ^
           (std::hash<T>()(k.a) << 2);
  }
};
