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
  uint8_t mOamAddr;

  struct {
    std::array<uint8_t, 8> low;
    std::array<uint8_t, 8> high;
  } mSpriteShifterPattern;

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

  std::array<std::array<uint8_t, 1024>, 2> mNameTable;
  std::array<uint8_t, 32> mPaletteTable;

  StatusRegister mStatusRegister;
  MaskRegister mMaskRegister;
  ControlRegister mControlRegister;

  bool mAddressLatch;
  uint8_t mPpuDataBuffer;

  LoopyRegister mVRamAddr;
  LoopyRegister mTRamAddr;

  uint8_t mFineX;

  struct {
    uint8_t nextTileId;
    uint8_t nextTileAttribute;
    uint8_t nextTileLsb;
    uint8_t nextTileMsb;
    uint16_t shifterPatternLow;
    uint16_t shifterPatternHigh;
    uint16_t shifterAttributeLow;
    uint16_t shifterAttributeHigh;
  } mBg;

  std::array<ObjectAttributeEntry, 64> mOam;
  std::array<ObjectAttributeEntry, 8> mSpriteOnScanline;
  uint8_t mSpriteCount;

  bool mSpriteZeroHitPossible;
  bool mSpriteZeroBeingRendered;

  IPpuState* mState;

  RenderingState mRenderingState;
  VerticalBlankState mVerticalBlankState;
  HorizontalBlankState mHorizontalBlankState;

  friend RenderingState;
  friend VerticalBlankState;
  friend HorizontalBlankState;
};
