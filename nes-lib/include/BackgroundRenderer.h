#pragma once
#include "PixelInfo.h"
#include "PpuRegisterDefinitions.h"
#include "Shifter.h"
#include <cstdint>

class Ppu;
class BackgroundRenderer {
 public:
  void Clock();
  void Reset();
  BackgroundPixelInfo CalculateBackgroundPixelInfo();

  bool GetRenderBackgroundFlag();
  bool GetRenderSpritesFlag();
  bool GetRenderBackgroundLeftFlag();
  bool GetRenderSpritesLeftFlag();
  bool GetGrayscale();
  bool GetSpriteSizeFlag();
  bool GetPatternSpriteFlag();

  uint8_t ReadStatus(uint16_t addr);
  uint8_t ReadPpuData(uint16_t addr);

  void WriteControl(uint16_t addr, uint8_t data);
  void WriteMask(uint16_t addr, uint8_t data);
  void WriteScroll(uint16_t addr, uint8_t data);
  void WritePpuAddress(uint16_t addr, uint8_t data);
  void WritePpuData(uint16_t addr, uint8_t data);
  void SetSpriteOverflowFlag(bool overflow);
  void SetSpriteZeroHitFlag(bool flag);

  void UpdateShifters();
  void SetPpu(Ppu* ppu);

 private:
  void VRamFetch();
  void FetchNametableTableByte();
  void FetchAttributeTableByte();
  void FetchPatternTableTileHigh();
  void FetchPatternTableTileLow();
  int CalculateNextTileIdAddres(uint8_t offset = 0);
  void IncrementScrollY();
  void IncrementScrollX();
  void TransferAddressY();
  void TransferAddressX();
  void LoadBackgroundShifters();

  void HorizontalBlankState();
  void RenderingState();
  void VerticalBlankState();

  Ppu* mPpu;

  StatusRegister mStatusRegister;
  MaskRegister mMaskRegister;
  ControlRegister mControlRegister;

  bool mAddressLatch;
  uint8_t mPpuDataBuffer;

  LoopyRegister mVRamAddr;
  LoopyRegister mTRamAddr;

  uint8_t mFineX;

  uint8_t nextTileId;
  uint8_t nextTileAttribute;
  uint8_t nextTileLsb;
  uint8_t nextTileMsb;
  Shifter patternShifter;
  Shifter attributeShifter;
};
