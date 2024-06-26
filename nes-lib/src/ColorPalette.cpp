#include "ColorPalette.h"
#include <algorithm>

std::unique_ptr<ColorPaletteU8> MakePixelColors() {
  auto pixelColors = std::make_unique<ColorPaletteU8>();

  pixelColors->at(0x00) = {84, 84, 84, 255};
  pixelColors->at(0x01) = {0, 30, 116, 255};
  pixelColors->at(0x02) = {8, 16, 144, 255};
  pixelColors->at(0x03) = {48, 0, 136, 255};
  pixelColors->at(0x04) = {68, 0, 100, 255};
  pixelColors->at(0x05) = {92, 0, 48, 255};
  pixelColors->at(0x06) = {84, 4, 0, 255};
  pixelColors->at(0x07) = {60, 24, 0, 255};
  pixelColors->at(0x08) = {32, 42, 0, 255};
  pixelColors->at(0x09) = {8, 58, 0, 255};
  pixelColors->at(0x0A) = {0, 64, 0, 255};
  pixelColors->at(0x0B) = {0, 60, 0, 255};
  pixelColors->at(0x0C) = {0, 50, 60, 255};
  pixelColors->at(0x0D) = {0, 0, 0, 255};
  pixelColors->at(0x0E) = {0, 0, 0, 255};
  pixelColors->at(0x0F) = {0, 0, 0, 255};

  pixelColors->at(0x10) = {152, 150, 152, 255};
  pixelColors->at(0x11) = {8, 76, 196, 255};
  pixelColors->at(0x12) = {48, 50, 236, 255};
  pixelColors->at(0x13) = {92, 30, 228, 255};
  pixelColors->at(0x14) = {136, 20, 176, 255};
  pixelColors->at(0x15) = {160, 20, 100, 255};
  pixelColors->at(0x16) = {152, 34, 32, 255};
  pixelColors->at(0x17) = {120, 60, 0, 255};
  pixelColors->at(0x18) = {84, 90, 0, 255};
  pixelColors->at(0x19) = {40, 114, 0, 255};
  pixelColors->at(0x1A) = {8, 124, 0, 255};
  pixelColors->at(0x1B) = {0, 118, 40, 255};
  pixelColors->at(0x1C) = {0, 102, 120, 255};
  pixelColors->at(0x1D) = {0, 0, 0, 255};
  pixelColors->at(0x1E) = {0, 0, 0, 255};
  pixelColors->at(0x1F) = {0, 0, 0, 255};

  pixelColors->at(0x20) = {236, 238, 236, 255};
  pixelColors->at(0x21) = {76, 154, 236, 255};
  pixelColors->at(0x22) = {120, 124, 236, 255};
  pixelColors->at(0x23) = {176, 98, 236, 255};
  pixelColors->at(0x24) = {228, 84, 236, 255};
  pixelColors->at(0x25) = {236, 88, 180, 255};
  pixelColors->at(0x26) = {236, 106, 100, 255};
  pixelColors->at(0x27) = {212, 136, 32, 255};
  pixelColors->at(0x28) = {160, 170, 0, 255};
  pixelColors->at(0x29) = {116, 196, 0, 255};
  pixelColors->at(0x2A) = {76, 208, 32, 255};
  pixelColors->at(0x2B) = {56, 204, 108, 255};
  pixelColors->at(0x2C) = {56, 180, 204, 255};
  pixelColors->at(0x2D) = {60, 60, 60, 255};
  pixelColors->at(0x2E) = {0, 0, 0, 255};
  pixelColors->at(0x2F) = {0, 0, 0, 255};

  pixelColors->at(0x30) = {236, 238, 236, 255};
  pixelColors->at(0x31) = {168, 204, 236, 255};
  pixelColors->at(0x32) = {188, 188, 236, 255};
  pixelColors->at(0x33) = {212, 178, 236, 255};
  pixelColors->at(0x34) = {236, 174, 236, 255};
  pixelColors->at(0x35) = {236, 174, 212, 255};
  pixelColors->at(0x36) = {236, 180, 176, 255};
  pixelColors->at(0x37) = {228, 196, 144, 255};
  pixelColors->at(0x38) = {204, 210, 120, 255};
  pixelColors->at(0x39) = {180, 222, 120, 255};
  pixelColors->at(0x3A) = {168, 226, 144, 255};
  pixelColors->at(0x3B) = {152, 226, 180, 255};
  pixelColors->at(0x3C) = {160, 214, 228, 255};
  pixelColors->at(0x3D) = {160, 162, 160, 255};
  pixelColors->at(0x3E) = {0, 0, 0, 255};
  pixelColors->at(0x3F) = {0, 0, 0, 255};

  return pixelColors;
}

std::unique_ptr<ColorPaletteF> MakePixelColorsFloat() {
  auto pixelColorsU8 = MakePixelColors();
  auto pixelColors = std::make_unique<ColorPaletteF>();
  std::transform(pixelColorsU8->begin(), pixelColorsU8->end(), pixelColors->data(),
                 [](PixelColorU8 color) -> PixelColorF {
                   return {static_cast<float>(color.r) / 255.f, static_cast<float>(color.g) / 255.f,
                           static_cast<float>(color.b) / 255.f, static_cast<float>(color.a) / 255.f};
                 });
  return pixelColors;
}
