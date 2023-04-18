#pragma once
#include "Cartridge.h"
#include "ColorPalette.h"
#include "FramePalette.h"
#include "IRenderer.h"
#include "ObjectAttributeEntry.h"
#include "PixelColor.h"
#include "PixelInfo.h"
#include "PpuPort.h"
#include "PpuRegisterDefinitions.h"
#include "Sprite.h"
#include "ppu-states/HorizontalBlankState.h"
#include "ppu-states/IPpuState.h"
#include "ppu-states/RenderingState.h"
#include "ppu-states/VerticalBlankState.h"
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <utility>

#define PPU_RAM_START 0x2000
#define PPU_RAM_END 0x3FFF

#define PPU_NAMETABLE_START PPU_RAM_START
#define PPU_NAMETABLE_END 0x3EFF

#define FRAME_PALETTE_START 0x3F00
#define FRAME_PALETTE_END PPU_RAM_END

class Bus;

class Ppu {
 public:
  Ppu(IRenderer& renderer);
  ~Ppu() = default;

  uint8_t CpuRead(uint16_t addr);
  void CpuWrite(uint16_t addr, uint8_t data);

  uint8_t PpuRead(uint16_t addr);
  void PpuWrite(uint16_t addr, uint8_t data);

  void ConnectBus(Bus* bus);
  void InsertCartridge(Cartridge* cartridge);
  void Clock();
  void Reset();

  PixelColor& GetColorFromPalette(uint8_t palette, uint8_t pixel);
  void NonMaskableInterrupt();

  uint8_t* mOamPtr = (uint8_t*)mOam.data();

 private:
  PixelColor& CalculatePixelColor();

  void VRamFetch();
  void IncrementScrollY();
  void IncrementScrollX();
  void TransferAddressY();
  void TransferAddressX();
  void UpdateShifters();
  void LoadBackgroundShifters();
  void TransitionState();

  void WriteControl(uint16_t addr, uint8_t data);
  void WriteMask(uint16_t addr, uint8_t data);
  void WriteOamAddress(uint16_t addr, uint8_t data);
  void WriteOamData(uint16_t addr, uint8_t data);
  void WriteScroll(uint16_t addr, uint8_t data);
  void WritePpuAddress(uint16_t addr, uint8_t data);
  void WritePpuData(uint16_t addr, uint8_t data);

  uint8_t ReadStatus(uint16_t addr);
  uint8_t ReadOamData(uint16_t addr);
  uint8_t ReadPpuData(uint16_t addr);

  BackgroundPixelInfo CalculateBackgroundPixelInfo();
  ForegroundPixelInfo CalculateForegroundPixelInfo();
  PixelInfo DetermineActualPixelInfo(const BackgroundPixelInfo& backgroundPixelInfo,
                                     const ForegroundPixelInfo& foregroundPixelInfo);

  Cartridge* mCartridge;
  int16_t mCycle;
  int16_t mScanline;
  Bus* mBus;
  IRenderer& mRenderer;
  std::unique_ptr<ColorPalette> mColorPalette;

  std::map<PpuPort, std::function<void(uint16_t, uint8_t)>> mCpuWriteCases;
  std::map<PpuPort, std::function<uint8_t(uint16_t)>> mCpuReadCases;

  std::array<std::array<uint8_t, 1024>, 2> mNameTable;
  FramePalette mFramePalette;

  StatusRegister mStatusRegister;
  MaskRegister mMaskRegister;
  ControlRegister mControlRegister;

  bool mAddressLatch;
  uint8_t mPpuDataBuffer;

  LoopyRegister mVRamAddr;
  LoopyRegister mTRamAddr;

  uint8_t mFineX;

  struct {
    std::array<uint8_t, 8> low;
    std::array<uint8_t, 8> high;
  } mSpriteShifterPattern;

  struct {
    uint8_t nextTileId;
    uint8_t nextTileAttribute;
    uint8_t nextTileLsb;
    uint8_t nextTileMsb;
    uint16_t shifterPatternLow;
    uint16_t shifterPatternHigh;
    uint16_t shifterAttributeLow;
    uint16_t shifterAttributeHigh;
  } mBackground;

  std::array<ObjectAttributeEntry, 64> mOam;
  uint8_t mOamAddr;

  std::array<ObjectAttributeEntry, 8> mSpriteOnScanline;
  uint8_t mSpriteCount;

  struct {
    bool onScanline;
    bool beingRendered;
  } mSpriteZero;

  IPpuState* mState;

  RenderingState mRenderingState;
  VerticalBlankState mVerticalBlankState;
  HorizontalBlankState mHorizontalBlankState;

  friend RenderingState;
  friend VerticalBlankState;
  friend HorizontalBlankState;
};
