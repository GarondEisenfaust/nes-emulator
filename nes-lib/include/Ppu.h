#pragma once
#include "BackgroundRenderer.h"
#include "Cartridge.h"
#include "ColorPalette.h"
#include "FramePalette.h"
#include "IRenderer.h"
#include "ObjectAttributeEntry.h"
#include "PixelColor.h"
#include "PixelInfo.h"
#include "PpuPort.h"
#include "PpuRegisterDefinitions.h"
#include "Shifter.h"
#include <cstdint>
#include <memory>

#define PPU_RAM_START 0x2000
#define PPU_RAM_END 0x3FFF

#define PPU_NAMETABLE_START PPU_RAM_START
#define PPU_NAMETABLE_END 0x3EFF

#define FRAME_PALETTE_START 0x3F00
#define FRAME_PALETTE_END PPU_RAM_END

class Bus;
class ForegroundRenderer;

class Ppu {
 public:
  Ppu(IRenderer& renderer);
  ~Ppu() = default;

  uint8_t CpuRead(uint16_t addr);
  void CpuWrite(uint16_t addr, uint8_t data);

  uint8_t PpuRead(uint16_t addr);
  void PpuWrite(uint16_t addr, uint8_t data);

  void ConnectBus(Bus* bus);
  void InsertCartridge(std::shared_ptr<Cartridge> cartridge);
  void Clock();
  void Reset();
  void UpdateShifters();
  void WriteOamData(int index, uint8_t data);

  void SetSpriteOverflowFlag(bool overflow);
  bool GetSpriteSizeFlag();
  bool GetPatternSpriteFlag();

  void SetForegroundRenderer(ForegroundRenderer* foregroundRenderer);
  void SetBackgroundRenderer(BackgroundRenderer* backgroundRenderer);
  void ScanlineCounter();

  int16_t mCycle;
  int16_t mScanline;
  bool mNonMaskableInterrupt;

 private:
  uint8_t CalculatePixelColor();
  uint8_t GetColorFromPalette(uint8_t palette, uint8_t pixel);

  PixelInfo DetermineActualPixelInfo(const BackgroundPixelInfo& backgroundPixelInfo,
                                     const ForegroundPixelInfo& foregroundPixelInfo);

  void DetectSpriteZeroHit();
  bool IsRendering();
  bool IsRenderingVisibleScanlines();

  ForegroundRenderer* mForegroundRenderer;
  BackgroundRenderer* mBackgroundRenderer;
  std::shared_ptr<Cartridge> mCartridge;

  Bus* mBus;
  IRenderer& mRenderer;
  std::unique_ptr<ColorPaletteU8> mColorPalette;

  std::array<std::array<uint8_t, 1024>, 2> mNameTable;
  FramePalette mFramePalette;
  unsigned int mPpuCycle = 0;
  unsigned int mPpuCycleForFrame = 0;
};
