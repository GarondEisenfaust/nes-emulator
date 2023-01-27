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

  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);
  void CpuWrite(uint16_t addr, uint8_t data);

  uint8_t PpuRead(uint16_t addr, bool bReadOnly = false);
  void PpuWrite(uint16_t addr, uint8_t data);

  void ConnectBus(Bus* bus);
  void InsertCartridge(Cartridge* cartridge);
  void Clock();
  void Reset();

  PixelColor& GetColorFromPalette(uint8_t palette, uint8_t pixel);
  void WritePatternTableToImage(const char* path, uint8_t i, uint8_t palette);
  void WriteColorPaletteToImage(const char* path);

  bool mFrameComplete;
  bool nmi;

  uint8_t* mOamPtr = (uint8_t*)mOam;
  uint8_t mOamAddr = 0x00;
  uint8_t mSpriteShifterPatternLo[8];
  uint8_t mSpriteShifterPatternHi[8];

 private:
  Cartridge* mCartridge;
  int16_t mCycle;
  int16_t mScanline;
  Bus* mBus;
  Grid* mGrid;
  std::unique_ptr<ColorPalette> mColorPalette;

  uint8_t tblName[2][1024];
  uint8_t tblPattern[2][4096];
  uint8_t tblPalette[32];

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

  struct ObjectAttributeEntry {
    uint8_t y;
    uint8_t id;
    uint8_t attribute;
    uint8_t x;
  } mOam[64];

  ObjectAttributeEntry mSpriteOnScanline[8];
  uint8_t mSpriteCount;

  bool bSpriteZeroHitPossible = false;
  bool bSpriteZeroBeingRendered = false;
};
