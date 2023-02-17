#include "ColorPalette.h"

std::unique_ptr<ColorPalette> MakePixelColors() {
  auto pixelColors = std::make_unique<ColorPalette>();

  pixelColors->at(0x00) = PixelColor{84, 84, 84, 255};
  pixelColors->at(0x01) = PixelColor{0, 30, 116, 255};
  pixelColors->at(0x02) = PixelColor{8, 16, 144, 255};
  pixelColors->at(0x03) = PixelColor{48, 0, 136, 255};
  pixelColors->at(0x04) = PixelColor{68, 0, 100, 255};
  pixelColors->at(0x05) = PixelColor{92, 0, 48, 255};
  pixelColors->at(0x06) = PixelColor{84, 4, 0, 255};
  pixelColors->at(0x07) = PixelColor{60, 24, 0, 255};
  pixelColors->at(0x08) = PixelColor{32, 42, 0, 255};
  pixelColors->at(0x09) = PixelColor{8, 58, 0, 255};
  pixelColors->at(0x0A) = PixelColor{0, 64, 0, 255};
  pixelColors->at(0x0B) = PixelColor{0, 60, 0, 255};
  pixelColors->at(0x0C) = PixelColor{0, 50, 60, 255};
  pixelColors->at(0x0D) = PixelColor{0, 0, 0, 255};
  pixelColors->at(0x0E) = PixelColor{0, 0, 0, 255};
  pixelColors->at(0x0F) = PixelColor{0, 0, 0, 255};

  pixelColors->at(0x10) = PixelColor{152, 150, 152, 255};
  pixelColors->at(0x11) = PixelColor{8, 76, 196, 255};
  pixelColors->at(0x12) = PixelColor{48, 50, 236, 255};
  pixelColors->at(0x13) = PixelColor{92, 30, 228, 255};
  pixelColors->at(0x14) = PixelColor{136, 20, 176, 255};
  pixelColors->at(0x15) = PixelColor{160, 20, 100, 255};
  pixelColors->at(0x16) = PixelColor{152, 34, 32, 255};
  pixelColors->at(0x17) = PixelColor{120, 60, 0, 255};
  pixelColors->at(0x18) = PixelColor{84, 90, 0, 255};
  pixelColors->at(0x19) = PixelColor{40, 114, 0, 255};
  pixelColors->at(0x1A) = PixelColor{8, 124, 0, 255};
  pixelColors->at(0x1B) = PixelColor{0, 118, 40, 255};
  pixelColors->at(0x1C) = PixelColor{0, 102, 120, 255};
  pixelColors->at(0x1D) = PixelColor{0, 0, 0, 255};
  pixelColors->at(0x1E) = PixelColor{0, 0, 0, 255};
  pixelColors->at(0x1F) = PixelColor{0, 0, 0, 255};

  pixelColors->at(0x20) = PixelColor{236, 238, 236, 255};
  pixelColors->at(0x21) = PixelColor{76, 154, 236, 255};
  pixelColors->at(0x22) = PixelColor{120, 124, 236, 255};
  pixelColors->at(0x23) = PixelColor{176, 98, 236, 255};
  pixelColors->at(0x24) = PixelColor{228, 84, 236, 255};
  pixelColors->at(0x25) = PixelColor{236, 88, 180, 255};
  pixelColors->at(0x26) = PixelColor{236, 106, 100, 255};
  pixelColors->at(0x27) = PixelColor{212, 136, 32, 255};
  pixelColors->at(0x28) = PixelColor{160, 170, 0, 255};
  pixelColors->at(0x29) = PixelColor{116, 196, 0, 255};
  pixelColors->at(0x2A) = PixelColor{76, 208, 32, 255};
  pixelColors->at(0x2B) = PixelColor{56, 204, 108, 255};
  pixelColors->at(0x2C) = PixelColor{56, 180, 204, 255};
  pixelColors->at(0x2D) = PixelColor{60, 60, 60, 255};
  pixelColors->at(0x2E) = PixelColor{0, 0, 0, 255};
  pixelColors->at(0x2F) = PixelColor{0, 0, 0, 255};

  pixelColors->at(0x30) = PixelColor{236, 238, 236, 255};
  pixelColors->at(0x31) = PixelColor{168, 204, 236, 255};
  pixelColors->at(0x32) = PixelColor{188, 188, 236, 255};
  pixelColors->at(0x33) = PixelColor{212, 178, 236, 255};
  pixelColors->at(0x34) = PixelColor{236, 174, 236, 255};
  pixelColors->at(0x35) = PixelColor{236, 174, 212, 255};
  pixelColors->at(0x36) = PixelColor{236, 180, 176, 255};
  pixelColors->at(0x37) = PixelColor{228, 196, 144, 255};
  pixelColors->at(0x38) = PixelColor{204, 210, 120, 255};
  pixelColors->at(0x39) = PixelColor{180, 222, 120, 255};
  pixelColors->at(0x3A) = PixelColor{168, 226, 144, 255};
  pixelColors->at(0x3B) = PixelColor{152, 226, 180, 255};
  pixelColors->at(0x3C) = PixelColor{160, 214, 228, 255};
  pixelColors->at(0x3D) = PixelColor{160, 162, 160, 255};
  pixelColors->at(0x3E) = PixelColor{0, 0, 0, 255};
  pixelColors->at(0x3F) = PixelColor{0, 0, 0, 255};

  return pixelColors;
}
