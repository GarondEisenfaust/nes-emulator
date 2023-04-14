#pragma once
#include <array>
#include <cstdint>

class FramePalette {
 public:
  uint8_t Read(uint16_t addr);
  uint8_t ReadGrayscale(uint16_t addr);
  void Write(uint16_t addr, uint8_t data);

 private:
  std::array<uint8_t, 32> mPalette;
  uint16_t DetermineFramePaletteAddress(uint16_t addr);
};
