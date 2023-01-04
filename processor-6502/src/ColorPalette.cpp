#include "ColorPalette.h"

std::unique_ptr<std::array<PixelColor, 56>> MakePixelColors() {
  auto pixelColors = std::make_unique<ColorPalette>();

  pixelColors->at(0) = PixelColor{84, 84, 84, 255};
  pixelColors->at(1) = PixelColor{0, 30, 116, 255};
  pixelColors->at(2) = PixelColor{8, 16, 144, 255};
  pixelColors->at(3) = PixelColor{48, 0, 136, 255};
  pixelColors->at(4) = PixelColor{68, 0, 100, 255};
  pixelColors->at(5) = PixelColor{92, 0, 48, 255};
  pixelColors->at(6) = PixelColor{84, 4, 0, 255};
  pixelColors->at(7) = PixelColor{60, 24, 0, 255};
  pixelColors->at(8) = PixelColor{32, 42, 0, 255};
  pixelColors->at(9) = PixelColor{8, 58, 0, 255};
  pixelColors->at(10) = PixelColor{0, 64, 0, 255};
  pixelColors->at(11) = PixelColor{0, 60, 0, 255};
  pixelColors->at(12) = PixelColor{0, 50, 60, 255};
  pixelColors->at(13) = PixelColor{0, 0, 0, 255};
  pixelColors->at(14) = PixelColor{152, 150, 152, 255};
  pixelColors->at(15) = PixelColor{8, 76, 196, 255};
  pixelColors->at(16) = PixelColor{48, 50, 236, 255};
  pixelColors->at(17) = PixelColor{92, 30, 228, 255};
  pixelColors->at(18) = PixelColor{136, 20, 176, 255};
  pixelColors->at(19) = PixelColor{160, 20, 100, 255};
  pixelColors->at(20) = PixelColor{152, 34, 32, 255};
  pixelColors->at(21) = PixelColor{120, 60, 0, 255};
  pixelColors->at(22) = PixelColor{84, 90, 0, 255};
  pixelColors->at(23) = PixelColor{40, 114, 0, 255};
  pixelColors->at(24) = PixelColor{8, 124, 0, 255};
  pixelColors->at(25) = PixelColor{0, 118, 40, 255};
  pixelColors->at(26) = PixelColor{0, 102, 120, 255};
  pixelColors->at(27) = PixelColor{0, 0, 0, 255};
  pixelColors->at(28) = PixelColor{236, 238, 236, 255};
  pixelColors->at(29) = PixelColor{76, 154, 236, 255};
  pixelColors->at(30) = PixelColor{120, 124, 236, 255};
  pixelColors->at(31) = PixelColor{176, 98, 236, 255};
  pixelColors->at(32) = PixelColor{228, 84, 236, 255};
  pixelColors->at(33) = PixelColor{236, 88, 180, 255};
  pixelColors->at(34) = PixelColor{236, 106, 100, 255};
  pixelColors->at(35) = PixelColor{212, 136, 32, 255};
  pixelColors->at(36) = PixelColor{160, 170, 0, 255};
  pixelColors->at(37) = PixelColor{116, 196, 0, 255};
  pixelColors->at(38) = PixelColor{76, 208, 32, 255};
  pixelColors->at(39) = PixelColor{56, 204, 108, 255};
  pixelColors->at(40) = PixelColor{56, 180, 204, 255};
  pixelColors->at(41) = PixelColor{60, 60, 60, 255};
  pixelColors->at(42) = PixelColor{236, 238, 236, 255};
  pixelColors->at(43) = PixelColor{168, 204, 236, 255};
  pixelColors->at(44) = PixelColor{188, 188, 236, 255};
  pixelColors->at(45) = PixelColor{212, 178, 236, 255};
  pixelColors->at(46) = PixelColor{236, 174, 236, 255};
  pixelColors->at(47) = PixelColor{236, 174, 212, 255};
  pixelColors->at(48) = PixelColor{236, 180, 176, 255};
  pixelColors->at(49) = PixelColor{228, 196, 144, 255};
  pixelColors->at(50) = PixelColor{204, 210, 120, 255};
  pixelColors->at(51) = PixelColor{180, 222, 120, 255};
  pixelColors->at(52) = PixelColor{168, 226, 144, 255};
  pixelColors->at(53) = PixelColor{152, 226, 180, 255};
  pixelColors->at(54) = PixelColor{160, 214, 228, 255};
  pixelColors->at(55) = PixelColor{160, 162, 160, 255};

  return pixelColors;
}
