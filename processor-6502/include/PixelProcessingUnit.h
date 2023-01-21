#pragma once
#include "Cartridge.h"
#include "ColorPalette.h"
#include "PixelColor.h"
#include "PixelProcessingUnitRegisterDefinitions.h"
#include "Sprite.h"
#include <cstdint>
#include <utility>

#define PPU_RAM_START 0x2000
#define PPU_RAM_END 0x3FFF
#define PPU_RAM_SIZE 0x0007

class Bus;
class Grid;

using PatternMemory = std::array<std::array<uint8_t, 128 * 128>, 2>;

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
  void Reset();

  Sprite& GetPatternTable(uint8_t i, uint8_t palette);
  PixelColor& GetColorFromPalette(uint8_t palette, uint8_t pixel);
  bool mFrameComplete;
  bool nmi;

 private:
  Cartridge* mCartridge;
  int mCycle;
  int mScanline;
  Bus* mBus;
  Grid* mGrid;
  std::unique_ptr<ColorPalette> mColorPalette;
  std::unique_ptr<PatternMemory> mPatternMemory;
  uint8_t tblName[2][1024];
  uint8_t tblPalette[32];
  uint8_t tblPattern[2][4096];

  StatusRegister mStatusRegister;
  MaskRegister mMaskRegister;
  ControlRegister mControlRegister;

  uint8_t mAddressLatch = 0x00;
  uint8_t mPpuDataBuffer = 0x00;

  LoopyRegister vRamAddr;
  LoopyRegister tRamAddr;

  uint8_t fineX = 0x00;

  uint8_t mBgNextTileId = 0x00;
  uint8_t mBgNextTileAttribute = 0x00;
  uint8_t mBgNextTileLsb = 0x00;
  uint8_t mBgNextTileMsb = 0x00;
  uint16_t mBgShifterPatternLow = 0x0000;
  uint16_t mBgShifterPatternHigh = 0x0000;
  uint16_t mBgShifterAttributeLow = 0x0000;
  uint16_t mBgShifterAttributeHigh = 0x0000;
};
