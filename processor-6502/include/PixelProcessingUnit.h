#pragma once
#include "Cartridge.h"
#include <cstdint>

#define PPU_RAM_START 0x2000
#define PPU_RAM_END 0x2000
#define PPU_RAM_SIZE 0x0008

class PixelProcessingUnit {
 public:
  PixelProcessingUnit() = default;
  ~PixelProcessingUnit() = default;

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

  void PpuWrite(uint16_t addr, uint8_t data);
  uint8_t PpuRead(uint16_t addr, bool bReadOnly = false);

  void InsertCartridge(Cartridge* cartridge);
  void Clock();

 private:
  Cartridge* mCartridge;
};
