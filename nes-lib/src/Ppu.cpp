#include "Ppu.h"
#include "Bus.h"
#include "Definitions.h"
#include "ForegroundRenderer.h"
#include "IRenderer.h"
#include "Mirror.h"
#include "PixelInfo.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <vector>

Ppu::Ppu(IRenderer& renderer)
    : mCycle(0), mScanline(0), mColorPalette(std::move(MakePixelColors())), mRenderer(renderer) {}

void Ppu::CpuWrite(uint16_t addr, uint8_t data) {
  auto addressedPort = static_cast<PpuPort>(addr % PPU_NUM_PORTS);

  switch (addressedPort) {
    case Control: {
      mBackgroundRenderer->WriteControl(addr, data);
      return;
    }
    case Mask: {
      mBackgroundRenderer->WriteMask(addr, data);
      return;
    }
    case OamAddress: {
      mForegroundRenderer->WriteOamAddress(addr, data);
      return;
    }
    case OamData: {
      mForegroundRenderer->WriteOamData(addr, data);
      return;
    }
    case Scroll: {
      mBackgroundRenderer->WriteScroll(addr, data);
      return;
    }
    case PpuAddress: {
      mBackgroundRenderer->WritePpuAddress(addr, data);
      return;
    }
    case PpuData: {
      mBackgroundRenderer->WritePpuData(addr, data);
      return;
    }
    default: {
      return;
    }
  }
}

uint8_t Ppu::CpuRead(uint16_t addr) {
  auto addressedPort = static_cast<PpuPort>(addr % PPU_NUM_PORTS);

  switch (addressedPort) {
    case Status: {
      return mBackgroundRenderer->ReadStatus(addr);
    }
    case OamData: {
      return mForegroundRenderer->ReadOamData(addr);
    }
    case PpuData: {
      return mBackgroundRenderer->ReadPpuData(addr);
    }
    default: {
      return 0;
    }
  }
}

void Ppu::PpuWrite(uint16_t addr, uint8_t data) {
  if (PPU_CARTRIDGE_START <= addr && addr <= PPU_CARTRIDGE_END) {
    mCartridge->PpuWrite(addr, data);
  } else if (PPU_NAMETABLE_START <= addr && addr <= PPU_NAMETABLE_END) {
    Mirror(mNameTable, mCartridge->GetMirrorMode(), addr) = data;
  } else if (FRAME_PALETTE_START <= addr && addr <= FRAME_PALETTE_END) {
    mFramePalette.Write(addr, data);
  }
}

uint8_t Ppu::PpuRead(uint16_t addr) {
  uint8_t data = 0x00;
  if (PPU_CARTRIDGE_START <= addr && addr <= PPU_CARTRIDGE_END) {
    data = mCartridge->PpuRead(addr);
  } else if (PPU_NAMETABLE_START <= addr && addr <= PPU_NAMETABLE_END) {
    data = Mirror(mNameTable, mCartridge->GetMirrorMode(), addr);
  } else if (FRAME_PALETTE_START <= addr && addr <= FRAME_PALETTE_END) {
    data = mBackgroundRenderer->GetGrayscale() ? mFramePalette.ReadGrayscale(addr) : mFramePalette.Read(addr);
  }
  return data;
}

void Ppu::InsertCartridge(std::shared_ptr<Cartridge> cartridge) { mCartridge = cartridge; }

void Ppu::Clock() {
  mBackgroundRenderer->Clock();
  mForegroundRenderer->Clock();

  uint16_t xPosOnScreen = mCycle - 1;
  uint16_t yPosOnScreen = mScanline;
  auto color = CalculatePixelColor();

  mRenderer.SetNesPixel(xPosOnScreen, yPosOnScreen, color);

  mCycle++;
  mPpuCycle++;

  if (IsRenderingVisibleScanlines() && mCycle == 260) {
    mCartridge->ScanlineCounter();
  }

  if (mCycle >= 341) {
    mCycle = 0;
    mScanline++;
    if (mScanline >= 261) {
      mScanline = -1;
      mRenderer.CommitFrame(mPpuCycleForFrame);
      mPpuCycleForFrame = mPpuCycle;
    }
  }
}

void Ppu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mPpu = this;
}

uint8_t Ppu::GetColorFromPalette(uint8_t palette, uint8_t pixel) {
  auto address = FRAME_PALETTE_START + (palette << 2) + pixel;
  return PpuRead(address);
}

void Ppu::SetSpriteOverflowFlag(bool overflow) { mBackgroundRenderer->SetSpriteOverflowFlag(overflow); }

bool Ppu::GetSpriteSizeFlag() { return mBackgroundRenderer->GetSpriteSizeFlag(); }

bool Ppu::GetPatternSpriteFlag() { return mBackgroundRenderer->GetPatternSpriteFlag(); }

void Ppu::SetForegroundRenderer(ForegroundRenderer* foregroundRenderer) { mForegroundRenderer = foregroundRenderer; }

void Ppu::SetBackgroundRenderer(BackgroundRenderer* backgroundRenderer) { mBackgroundRenderer = backgroundRenderer; }

void Ppu::ScanlineCounter() { mCartridge->ScanlineCounter(); }

void Ppu::Reset() {
  mScanline = 0;
  mCycle = 0;
  mBackgroundRenderer->Reset();
}

uint8_t ReadValueFromBackgroundShifter(uint16_t highShifter, uint16_t lowShifter, uint16_t bitMux) {
  bool lowBit = (lowShifter & bitMux);
  bool highBit = (highShifter & bitMux);
  return (highBit << 1) | lowBit;
}

PixelInfo Ppu::DetermineActualPixelInfo(const BackgroundPixelInfo& backgroundPixelInfo,
                                        const ForegroundPixelInfo& foregroundPixelInfo) {
  PixelInfo result;
  if (backgroundPixelInfo.pixel == 0 && foregroundPixelInfo.pixel > 0) {
    result.pixel = foregroundPixelInfo.pixel;
    result.palette = foregroundPixelInfo.palette;
  } else if (backgroundPixelInfo.pixel > 0 && foregroundPixelInfo.pixel == 0) {
    result.pixel = backgroundPixelInfo.pixel;
    result.palette = backgroundPixelInfo.palette;
  } else if (backgroundPixelInfo.pixel > 0 && foregroundPixelInfo.pixel > 0) {
    if (foregroundPixelInfo.priority) {
      result.pixel = foregroundPixelInfo.pixel;
      result.palette = foregroundPixelInfo.palette;
    } else {
      result.pixel = backgroundPixelInfo.pixel;
      result.palette = backgroundPixelInfo.palette;
    }

    DetectSpriteZeroHit();
  }
  return result;
}

void Ppu::DetectSpriteZeroHit() {
  if (mForegroundRenderer->GetSpriteZeroOnScanlineFlag() && mForegroundRenderer->GetSpriteZeroBeingRenderedFlag() &&
      mBackgroundRenderer->GetRenderBackgroundFlag() && mBackgroundRenderer->GetRenderSpritesFlag()) {
    if (!mBackgroundRenderer->GetRenderBackgroundLeftFlag() && !mBackgroundRenderer->GetRenderSpritesLeftFlag() &&
        9 <= mCycle && mCycle < 258) {
      mBackgroundRenderer->SetSpriteZeroHitFlag(true);
    } else if (1 <= mCycle && mCycle < 258) {
      mBackgroundRenderer->SetSpriteZeroHitFlag(true);
    }
  }
}

bool Ppu::IsRendering() {
  return mBackgroundRenderer->GetRenderBackgroundFlag() || mBackgroundRenderer->GetRenderSpritesFlag();
}

bool Ppu::IsRenderingVisibleScanlines() { return IsRendering() && 0 <= mScanline && mScanline < 240; }

uint8_t Ppu::CalculatePixelColor() {
  auto backgroundInfo = mBackgroundRenderer->CalculateBackgroundPixelInfo();

  ForegroundPixelInfo foregroundInfo;
  if (mBackgroundRenderer->GetRenderSpritesFlag()) {
    foregroundInfo = mForegroundRenderer->CalculateForegroundPixelInfo();
  }
  auto pixelInfo = DetermineActualPixelInfo(backgroundInfo, foregroundInfo);

  return GetColorFromPalette(pixelInfo.palette, pixelInfo.pixel);
};

void Ppu::UpdateShifters() {
  mBackgroundRenderer->UpdateShifters();
  if (mBackgroundRenderer->GetRenderBackgroundFlag() && mCycle >= 1 && mCycle < 258) {
    mForegroundRenderer->UpdateShifters();
  }
}

void Ppu::WriteOamData(int index, uint8_t data) { mForegroundRenderer->WriteOamData(index, data); }
