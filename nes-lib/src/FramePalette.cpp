#include "FramePalette.h"
#include "ColorPalette.h"

uint16_t FramePalette::DetermineFramePaletteAddress(uint16_t addr) {
  addr %= mPalette.size();
  if (addr == 0x0010) {
    addr = 0x0000;
  }
  if (addr == 0x0014) {
    addr = 0x0004;
  }
  if (addr == 0x0018) {
    addr = 0x0008;
  }
  if (addr == 0x001C) {
    addr = 0x000C;
  }
  return addr;
}

uint8_t FramePalette::Read(uint16_t addr) {
  addr = DetermineFramePaletteAddress(addr);
  return mPalette[addr] % NUMBER_OF_COLORS;
}

uint8_t FramePalette::ReadGrayscale(uint16_t addr) { return Read(addr) & 0x30; }

void FramePalette::Write(uint16_t addr, uint8_t data) {
  addr = DetermineFramePaletteAddress(addr);
  mPalette[addr] = data;
}
