#pragma once
#include "Cartridge.h"
#include "ColorPalette.h"
#include "IRenderer.h"
#include "ObjectAttributeEntry.h"
#include "PixelColor.h"
#include "PpuRegisterDefinitions.h"
#include "Sprite.h"
#include "ppu-states/HorizontalBlankState.h"
#include "ppu-states/IPpuState.h"
#include "ppu-states/RenderingState.h"
#include "ppu-states/VerticalBlankState.h"
#include <cstdint>
#include <memory>
#include <utility>

#define PPU_RAM_START 0x2000
#define PPU_RAM_END 0x3FFF
#define PPU_RAM_SIZE 0x0007

class Bus;
class Grid;

using PatternMemory = std::array<std::array<uint8_t, 128 * 128>, 2>;

class Ppu {
 public:
  Ppu(IRenderer& renderer);
  ~Ppu() = default;

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

  bool nmi;

  uint8_t* mOamPtr = (uint8_t*)mOam.data();
  uint8_t mOamAddr = 0x00;
  std::array<uint8_t, 8> mSpriteShifterPatternLo;
  std::array<uint8_t, 8> mSpriteShifterPatternHi;

 private:
  PixelColor& CalculatePixelColor();

  void VRamFetch();
  void IncrementScrollY();
  void IncrementScrollX();
  void TransferAddressY();
  void TransferAddressX();
  void UpdateShifters();
  void LoadBackgroundShifters();

  void Transition();

  Cartridge* mCartridge;
  int16_t mCycle;
  int16_t mScanline;
  Bus* mBus;
  IRenderer& mRenderer;
  std::unique_ptr<ColorPalette> mColorPalette;

  std::array<std::array<uint8_t, 1024>, 2> tblName;
  std::array<std::array<uint8_t, 4096>, 2> tblPattern;
  std::array<uint8_t, 32> tblPalette;

  StatusRegister mStatusRegister;
  MaskRegister mMaskRegister;
  ControlRegister mControlRegister;

  bool mAddressLatch = false;
  uint8_t mPpuDataBuffer = 0x00;

  LoopyRegister vRamAddr;
  LoopyRegister tRamAddr;

  uint8_t fineX = 0x00;

  struct {
    uint8_t nextTileId = 0x00;
    uint8_t nextTileAttribute = 0x00;
    uint8_t nextTileLsb = 0x00;
    uint8_t nextTileMsb = 0x00;
    uint16_t shifterPatternLow = 0x0000;
    uint16_t shifterPatternHigh = 0x0000;
    uint16_t shifterAttributeLow = 0x0000;
    uint16_t shifterAttributeHigh = 0x0000;
  } mBg;

  std::array<ObjectAttributeEntry, 64> mOam;
  std::array<ObjectAttributeEntry, 8> mSpriteOnScanline;
  uint8_t mSpriteCount;

  bool bSpriteZeroHitPossible = false;
  bool bSpriteZeroBeingRendered = false;

  IPpuState* mState;

  RenderingState mRenderingState;
  VerticalBlankState mVerticalBlankState;
  HorizontalBlankState mHorizontalBlankState;

  friend RenderingState;
  friend VerticalBlankState;
  friend HorizontalBlankState;
};
