#include "Ppu.h"
#include "Bus.h"
#include "Definitions.h"
#include "IRenderer.h"
#include "Mirror.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

Ppu::Ppu(IRenderer& renderer)
    : mCycle(0),
      mScanline(0),
      mColorPalette(std::move(MakePixelColors())),
      mRenderer(renderer),
      mRenderingState(*this),
      mVerticalBlankState(*this),
      mHorizontalBlankState(*this),
      mState(&mRenderingState) {
  mCpuWriteCases.emplace(PpuPort::Control, [&](uint16_t addr, uint8_t data) { WriteControl(addr, data); });
  mCpuWriteCases.emplace(PpuPort::Mask, [&](uint16_t addr, uint8_t data) { WriteMask(addr, data); });
  mCpuWriteCases.emplace(PpuPort::OamAddress, [&](uint16_t addr, uint8_t data) { WriteOamAddress(addr, data); });
  mCpuWriteCases.emplace(PpuPort::OamData, [&](uint16_t addr, uint8_t data) { WriteOamData(addr, data); });
  mCpuWriteCases.emplace(PpuPort::Scroll, [&](uint16_t addr, uint8_t data) { WriteScroll(addr, data); });
  mCpuWriteCases.emplace(PpuPort::PpuAddress, [&](uint16_t addr, uint8_t data) { WritePpuAddress(addr, data); });
  mCpuWriteCases.emplace(PpuPort::PpuData, [&](uint16_t addr, uint8_t data) { WritePpuData(addr, data); });

  mCpuReadCases.emplace(PpuPort::Status, [&](uint16_t addr) { return ReadStatus(addr); });
  mCpuReadCases.emplace(PpuPort::OamData, [&](uint16_t addr) { return ReadOamData(addr); });
  mCpuReadCases.emplace(PpuPort::PpuData, [&](uint16_t addr) { return ReadPpuData(addr); });
}

void Ppu::CpuWrite(uint16_t addr, uint8_t data) {
  auto addressedPort = static_cast<PpuPort>(addr % PPU_NUM_PORTS);
  mCpuWriteCases.at(addressedPort)(addr, data);
}

uint8_t Ppu::CpuRead(uint16_t addr) {
  auto addressedPort = static_cast<PpuPort>(addr % PPU_NUM_PORTS);
  return mCpuReadCases.at(addressedPort)(addr);
}

void Ppu::PpuWrite(uint16_t addr, uint8_t data) {
  if (PPU_CARTRIDGE_START <= addr && addr <= PPU_CARTRIDGE_END) {
    mCartridge->PpuWrite(addr, data);
  } else if (PPU_NAMETABLE_START <= addr && addr <= PPU_NAMETABLE_END) {
    Mirror(mNameTable, mCartridge->mMirror, addr) = data;
  } else if (FRAME_PALETTE_START <= addr && addr <= FRAME_PALETTE_END) {
    mFramePalette.Write(addr, data);
  }
}

uint8_t Ppu::PpuRead(uint16_t addr) {
  uint8_t data = 0x00;
  if (PPU_CARTRIDGE_START <= addr && addr <= PPU_CARTRIDGE_END) {
    data = mCartridge->PpuRead(addr);
  } else if (PPU_NAMETABLE_START <= addr && addr <= PPU_NAMETABLE_END) {
    data = Mirror(mNameTable, mCartridge->mMirror, addr);
  } else if (FRAME_PALETTE_START <= addr && addr <= FRAME_PALETTE_END) {
    data = mMaskRegister.grayscale ? mFramePalette.ReadGrayscale(addr) : mFramePalette.Read(addr);
  }
  return data;
}

void Ppu::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }

void Ppu::Clock() {
  mState->Execute();

  auto xPosOnScreen = mCycle - 1;
  auto yPosOnScreen = mScanline;
  auto& color = CalculatePixelColor();
  mRenderer.SetPixelColor(xPosOnScreen, yPosOnScreen, color);

  mCycle++;

  if (mCycle >= 341) {
    mCycle = 0;
    mScanline++;
    if (mScanline >= 261) {
      mScanline = -1;
      mRenderer.CommitFrame();
    }
  }
  TransitionState();
}

void Ppu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mPpu = this;
}

PixelColor& Ppu::GetColorFromPalette(uint8_t palette, uint8_t pixel) {
  auto address = FRAME_PALETTE_START + (palette << 2) + pixel;
  auto index = PpuRead(address);
  return mColorPalette->at(index);
}

void Ppu::NonMaskableInterrupt() { mBus->NonMaskableInterrupt(); }

void Ppu::Reset() {
  mFineX = 0x00;
  mAddressLatch = 0x00;
  mPpuDataBuffer = 0x00;
  mScanline = 0;
  mCycle = 0;
  mBackground.nextTileId = 0x00;
  mBackground.nextTileAttribute = 0x00;
  mBackground.nextTileLsb = 0x00;
  mBackground.nextTileMsb = 0x00;
  mBackground.shifterPatternLow = 0x0000;
  mBackground.shifterPatternHigh = 0x0000;
  mBackground.shifterAttributeLow = 0x0000;
  mBackground.shifterAttributeHigh = 0x0000;
  mStatusRegister.reg = 0x00;
  mMaskRegister.reg = 0x00;
  mControlRegister.reg = 0x00;
  mVRamAddr.reg = 0x0000;
  mTRamAddr.reg = 0x0000;
  mState = &mRenderingState;
}

uint8_t ReadValueFromBackgroundShifter(uint16_t highShifter, uint16_t lowShifter, uint16_t bitMux) {
  bool lowBit = (lowShifter & bitMux) > 0;
  bool highBit = (highShifter & bitMux) > 0;
  return (highBit << 1) | lowBit;
}

BackgroundPixelInfo Ppu::CalculateBackgroundPixelInfo() {
  BackgroundPixelInfo result;

  if (!mMaskRegister.renderBackground) {
    return result;
  }
  constexpr uint16_t mostSignificantBit = 1 << 15;
  uint16_t bitMux = mostSignificantBit >> mFineX;
  result.pixel = ReadValueFromBackgroundShifter(mBackground.shifterPatternHigh, mBackground.shifterPatternLow, bitMux);
  result.palette =
      ReadValueFromBackgroundShifter(mBackground.shifterAttributeHigh, mBackground.shifterAttributeLow, bitMux);
  return result;
}

ForegroundPixelInfo Ppu::CalculateForegroundPixelInfo() {
  ForegroundPixelInfo result;

  if (!mMaskRegister.renderSprites) {
    return result;
  }
  mSpriteZero.beingRendered = false;

  for (uint8_t i = 0; i < mSpriteCount; i++) {
    if (mSpriteOnScanline[i].x != 0) {
      continue;
    }
    bool foregroudPixelPixelLow = (mSpriteShifterPattern.low[i] & (1 << 7)) > 0;
    bool foregroudPixelPixelHigh = (mSpriteShifterPattern.high[i] & (1 << 7)) > 0;
    result.pixel = (foregroudPixelPixelHigh << 1) | foregroudPixelPixelLow;

    result.palette = (mSpriteOnScanline[i].attribute & 0b11) + 0x04;
    result.priority = (mSpriteOnScanline[i].attribute & (1 << 5)) == 0;

    if (result.pixel == 0) {
      continue;
    }
    if (i == 0) {
      mSpriteZero.beingRendered = true;
    }
    break;
  }
  return result;
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

    if (mSpriteZero.onScanline && mSpriteZero.beingRendered && mMaskRegister.renderBackground &&
        mMaskRegister.renderSprites) {
      if (!mMaskRegister.renderBackgroundLeft && !mMaskRegister.renderSpritesLeft && 9 <= mCycle && mCycle < 258) {
        mStatusRegister.spriteZeroHit = true;
      } else if (1 <= mCycle && mCycle < 258) {
        mStatusRegister.spriteZeroHit = true;
      }
    }
  }
  return result;
}

PixelColor& Ppu::CalculatePixelColor() {
  auto backgroundInfo = CalculateBackgroundPixelInfo();
  auto foregroundInfo = CalculateForegroundPixelInfo();
  auto pixelInfo = DetermineActualPixelInfo(backgroundInfo, foregroundInfo);

  return GetColorFromPalette(pixelInfo.palette, pixelInfo.pixel);
};

void Ppu::VRamFetch() {
  UpdateShifters();
  switch ((mCycle - 1) % 8) {
    case 0: {
      LoadBackgroundShifters();
      mBackground.nextTileId = PpuRead(0x2000 | (mVRamAddr.reg & 0x0FFF));
      break;
    }
    case 2: {
      mBackground.nextTileAttribute = PpuRead(0x23C0 | (mVRamAddr.nameTableY << 11) | (mVRamAddr.nameTableX << 10) |
                                              ((mVRamAddr.coarseY >> 2) << 3) | (mVRamAddr.coarseX >> 2));

      if (mVRamAddr.coarseY & 0b10) {
        mBackground.nextTileAttribute >>= 4;
      }
      if (mVRamAddr.coarseX & 0b10) {
        mBackground.nextTileAttribute >>= 2;
      }
      mBackground.nextTileAttribute &= 0b11;
      break;
    }
    case 4: {
      auto toReadFrom = (mControlRegister.patternBackground << 12) +
                        (static_cast<uint16_t>(mBackground.nextTileId) << 4) + (mVRamAddr.fineY + 0);
      mBackground.nextTileLsb = PpuRead(toReadFrom);
      break;
    }
    case 6: {
      auto toReadFrom = (mControlRegister.patternBackground << 12) +
                        (static_cast<uint16_t>(mBackground.nextTileId) << 4) + (mVRamAddr.fineY + 8);
      mBackground.nextTileMsb = PpuRead(toReadFrom);
      break;
    }
    case 7: {
      IncrementScrollX();
      break;
    }
  }
}

void Ppu::IncrementScrollY() {
  if (!(mMaskRegister.renderBackground || mMaskRegister.renderSprites)) {
    return;
  }
  if (mVRamAddr.fineY < 7) {
    mVRamAddr.fineY++;
    return;
  }
  mVRamAddr.fineY = 0;
  if (mVRamAddr.coarseY == 29) {
    mVRamAddr.coarseY = 0;
    mVRamAddr.nameTableY = !mVRamAddr.nameTableY;
  } else if (mVRamAddr.coarseY == 31) {
    mVRamAddr.coarseY = 0;
  } else {
    mVRamAddr.coarseY++;
  }
}

void Ppu::IncrementScrollX() {
  if (!(mMaskRegister.renderBackground || mMaskRegister.renderSprites)) {
    return;
  }
  if (mVRamAddr.coarseX >= 31) {
    mVRamAddr.coarseX = 0;
    mVRamAddr.nameTableX = !mVRamAddr.nameTableX;
  } else {
    mVRamAddr.coarseX++;
  }
}

void Ppu::TransferAddressY() {
  if (!(mMaskRegister.renderBackground || mMaskRegister.renderSprites)) {
    return;
  }
  mVRamAddr.fineY = mTRamAddr.fineY;
  mVRamAddr.nameTableY = mTRamAddr.nameTableY;
  mVRamAddr.coarseY = mTRamAddr.coarseY;
}

void Ppu::TransferAddressX() {
  if (!(mMaskRegister.renderBackground || mMaskRegister.renderSprites)) {
    return;
  }
  mVRamAddr.nameTableX = mTRamAddr.nameTableX;
  mVRamAddr.coarseX = mTRamAddr.coarseX;
}

void Ppu::UpdateShifters() {
  if (mMaskRegister.renderBackground) {
    mBackground.shifterPatternLow <<= 1;
    mBackground.shifterPatternHigh <<= 1;

    mBackground.shifterAttributeLow <<= 1;
    mBackground.shifterAttributeHigh <<= 1;
  }
  if (!(mMaskRegister.renderBackground && mCycle >= 1 && mCycle < 258)) {
    return;
  }
  for (int i = 0; i < mSpriteCount; i++) {
    if (mSpriteOnScanline[i].x > 0) {
      mSpriteOnScanline[i].x--;
      continue;
    }
    mSpriteShifterPattern.low[i] <<= 1;
    mSpriteShifterPattern.high[i] <<= 1;
  }
}

void Ppu::LoadBackgroundShifters() {
  mBackground.shifterPatternLow = (mBackground.shifterPatternLow & 0xFF00) | mBackground.nextTileLsb;
  mBackground.shifterPatternHigh = (mBackground.shifterPatternHigh & 0xFF00) | mBackground.nextTileMsb;

  mBackground.shifterAttributeLow =
      (mBackground.shifterAttributeLow & 0xFF00) | ((mBackground.nextTileAttribute & 0b01) ? 0xFF : 0x00);
  mBackground.shifterAttributeHigh =
      (mBackground.shifterAttributeHigh & 0xFF00) | ((mBackground.nextTileAttribute & 0b10) ? 0xFF : 0x00);
};

void Ppu::TransitionState() {
  if (mScanline >= 240) {
    mState = &mVerticalBlankState;
  } else if (mCycle >= 256) {
    mState = &mHorizontalBlankState;
  } else {
    mState = &mRenderingState;
  }
}

void Ppu::WriteControl(uint16_t addr, uint8_t data) {
  mControlRegister.reg = data;
  mTRamAddr.nameTableX = mControlRegister.nametableX;
  mTRamAddr.nameTableY = mControlRegister.nametableY;
}

void Ppu::WriteMask(uint16_t addr, uint8_t data) { mMaskRegister.reg = data; }

void Ppu::WriteOamAddress(uint16_t addr, uint8_t data) { mOamAddr = data; }

void Ppu::WriteOamData(uint16_t addr, uint8_t data) { mOamPtr[mOamAddr] = data; }

void Ppu::WriteScroll(uint16_t addr, uint8_t data) {
  if (!mAddressLatch) {
    mFineX = data & 0b111;
    mTRamAddr.coarseX = data >> 3;
    mAddressLatch = true;
  } else {
    mTRamAddr.fineY = data & 0b111;
    mTRamAddr.coarseY = data >> 3;
    mAddressLatch = false;
  }
}

void Ppu::WritePpuAddress(uint16_t addr, uint8_t data) {
  if (!mAddressLatch) {
    mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0x00FF) | (static_cast<uint16_t>(data & 0b111111) << 8);
    mAddressLatch = true;
  } else {
    mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0xFF00) | static_cast<uint16_t>(data);
    mVRamAddr = mTRamAddr;
    mAddressLatch = false;
  }
}

void Ppu::WritePpuData(uint16_t addr, uint8_t data) {
  PpuWrite(mVRamAddr.reg, data);
  mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
}

uint8_t Ppu::ReadStatus(uint16_t addr) {
  auto data = (mStatusRegister.reg & 0xE0) | (mPpuDataBuffer & 0x1F);
  mStatusRegister.verticalBlank = false;
  mAddressLatch = false;
  return data;
}

uint8_t Ppu::ReadOamData(uint16_t addr) { return mOamPtr[mOamAddr]; }

uint8_t Ppu::ReadPpuData(uint16_t addr) {
  auto data = mPpuDataBuffer;
  mPpuDataBuffer = PpuRead(mVRamAddr.reg);
  if (mVRamAddr.reg >= FRAME_PALETTE_START) {
    data = mPpuDataBuffer;
  }
  mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
  return data;
}
