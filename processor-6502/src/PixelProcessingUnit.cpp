#include "PixelProcessingUnit.h"
#include "Bus.h"
#include "ColorPalette.h"
#include "Definitions.h"
#include "Grid.h"

PixelProcessingUnit::PixelProcessingUnit(Grid* grid)
    : mCycle(0), mScanline(0), mFrameComplete(false), mColorPalette(std::move(MakePixelColors())), mGrid(grid) {}

void PixelProcessingUnit::CpuWrite(uint16_t addr, uint8_t data) {
  switch (addr) {
    case 0x0000:  // Control
      break;
    case 0x0001:  // Mask
      break;
    case 0x0002:  // Status
      break;
    case 0x0003:  // OAM Address
      break;
    case 0x0004:  // OAM Data
      break;
    case 0x0005:  // Scroll
      break;
    case 0x0006:  // PPU Address
      break;
    case 0x0007:  // PPU Data
      break;
  }
}

uint8_t PixelProcessingUnit::CpuRead(uint16_t addr, bool bReadOnly) {
  switch (addr) {
    case 0x0000:  // Control
      break;
    case 0x0001:  // Mask
      break;
    case 0x0002:  // Status
      break;
    case 0x0003:  // OAM Address
      break;
    case 0x0004:  // OAM Data
      break;
    case 0x0005:  // Scroll
      break;
    case 0x0006:  // PPU Address
      break;
    case 0x0007:  // PPU Data
      break;
  }
  return UNDEFINED;
}

void PixelProcessingUnit::PpuWrite(uint16_t addr, uint8_t data) {
  addr &= 0x3FFF;
  if (mCartridge->CpuWrite(addr, data)) {
  }
}

uint8_t PixelProcessingUnit::PpuRead(uint16_t addr, bool bReadOnly) {
  addr &= 0x3FFF;
  if (mCartridge->CpuRead(addr).hasRead) {
  }
  return 0;
}

void PixelProcessingUnit::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }

void PixelProcessingUnit::Clock() {
  mGrid->GetPixel(mCycle, mScanline).SetColor(PixelColor{88, 88, 88, 100});
  mCycle++;

  if (mCycle >= 341) {
    mCycle = 0;
    mScanline++;
    if (mScanline >= 261) {
      mScanline = -1;
      mFrameComplete = true;
    }
  }
}

void PixelProcessingUnit::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mPpu = this;
}
