#pragma once
#include "Cartridge.h"
#include "PixelColor.h"
#include <cstdint>

#define PPU_RAM_START 0x2000
#define PPU_RAM_END 0x2000
#define PPU_RAM_SIZE 0x0008

class Bus;
class Grid;
class PixelProcessingUnit {
 public:
  PixelProcessingUnit(Grid* grid);
  ~PixelProcessingUnit() = default;

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

  void PpuWrite(uint16_t addr, uint8_t data);
  uint8_t PpuRead(uint16_t addr, bool bReadOnly = false);

  void ConnectBus(Bus* bus);
  void InsertCartridge(Cartridge* cartridge);
  void Clock();
  bool mFrameComplete;

 private:
  Cartridge* mCartridge;
  int mCycle;
  int mScanline;
  Bus* mBus;
  Grid* mGrid;
  std::unique_ptr<std::array<PixelColor, 56>> mColorPalette;
};
