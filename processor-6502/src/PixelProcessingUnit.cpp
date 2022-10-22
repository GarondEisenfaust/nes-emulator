#include "PixelProcessingUnit.h"
#include "Definitions.h"

void PixelProcessingUnit::CpuWrite(uint16_t addr, uint8_t data) {
  switch (addr) {
    case 0x0000:  // Control
      break;
    case 0x0001:  // Mask
      break;
    case 0x0002:  // Status
      break;
    case 0x0003:  // OAM Address
      break;
    case 0x0004:  // OAM Data
      break;
    case 0x0005:  // Scroll
      break;
    case 0x0006:  // PPU Address
      break;
    case 0x0007:  // PPU Data
      break;
  }
}

uint8_t PixelProcessingUnit::CpuRead(uint16_t addr, bool bReadOnly) {
  switch (addr) {
    case 0x0000:  // Control
      break;
    case 0x0001:  // Mask
      break;
    case 0x0002:  // Status
      break;
    case 0x0003:  // OAM Address
      break;
    case 0x0004:  // OAM Data
      break;
    case 0x0005:  // Scroll
      break;
    case 0x0006:  // PPU Address
      break;
    case 0x0007:  // PPU Data
      break;
  }
  return UNDEFINED;
}

void PixelProcessingUnit::PpuWrite(uint16_t addr, uint8_t data) {
  addr &= 0x3FFF;
  if (mCartridge->CpuWrite(addr, data)) {
  }
}

uint8_t PixelProcessingUnit::PpuRead(uint16_t addr, bool bReadOnly) {
  addr &= 0x3FFF;
  if (mCartridge->CpuRead(addr).hasRead) {
  }
  return 0;
}

void PixelProcessingUnit::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }

void PixelProcessingUnit::Clock() {}
