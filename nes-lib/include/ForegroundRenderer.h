#pragma once
#include "ObjectAttributeEntry.h"
#include "PixelInfo.h"
#include <array>
#include <cstdint>

class Ppu;
class ForegroundRenderer {
 public:
  ForegroundPixelInfo CalculateForegroundPixelInfo();
  void UpdateShifters();
  void Clock();
  void WriteOamAddress(uint16_t addr, uint8_t data);
  void WriteOamData(uint16_t addr, uint8_t data);
  uint8_t ReadOamData(uint16_t addr);
  void HorizontalBlankState();
  void LoadSpriteShifters(uint16_t spritePatternAddrLow, uint16_t spritePatternAddrHigh, uint8_t i);
  uint16_t GetLowAddressOfBigSprite(uint8_t i);
  uint16_t GetLowAddressOfSmallSprite(uint8_t i);
  void FlipSpriteIfNecessary(uint8_t attribute, uint8_t& spritePatternBitsLow, uint8_t& spritePatternBitsHigh);
  void DetectSpritesOnScanline();
  void RenderingState();
  void VerticalBlankState();
  void SetPpu(Ppu* ppu);

  bool GetSpriteZeroOnScanlineFlag();
  bool GetSpriteZeroBeingRenderedFlag();

 private:
  bool mSpriteZeroOnScanline;
  bool mSpriteZeroBeingRendered;

  uint8_t mOamAddr;
  Ppu* mPpu;
  std::array<ObjectAttributeEntry, 64> mOam;
  std::array<ObjectAttributeEntry, 8> mSpriteOnScanline;
  uint8_t mSpriteCount;

  struct {
    std::array<uint8_t, 8> low;
    std::array<uint8_t, 8> high;
  } mSpriteShifterPattern;
};
