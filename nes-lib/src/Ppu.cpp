#include "Ppu.h"
#include "Bus.h"
#include "Definitions.h"
#include "IRenderer.h"
#include "Mirror.h"
#include "PpuPort.h"
#include "lodepng.h"
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
      mState(&mRenderingState) {}

void Ppu::CpuWrite(uint16_t addr, uint8_t data) {
  addr %= PPU_NUM_PORTS;
  switch (addr) {
    case PpuPort::Control: {
      mControlRegister.reg = data;
      mTRamAddr.nameTableX = mControlRegister.nametableX;
      mTRamAddr.nameTableY = mControlRegister.nametableY;
      break;
    }
    case PpuPort::Mask: {
      mMaskRegister.reg = data;
      break;
    }
    case PpuPort::OamAddress: {
      mOamAddr = data;
      break;
    }
    case PpuPort::OamData: {
      mOamPtr[mOamAddr] = data;
      break;
    }
    case PpuPort::Scroll: {
      if (!mAddressLatch) {
        mFineX = data & 0b111;
        mTRamAddr.coarseX = data >> 3;
        mAddressLatch = true;
      } else {
        mTRamAddr.fineY = data & 0b111;
        mTRamAddr.coarseY = data >> 3;
        mAddressLatch = false;
      }
      break;
    }
    case PpuPort::PpuAddress: {
      if (!mAddressLatch) {
        mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0x00FF) | (static_cast<uint16_t>(data & 0b111111) << 8);
        mAddressLatch = true;
      } else {
        mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0xFF00) | static_cast<uint16_t>(data);
        mVRamAddr = mTRamAddr;
        mAddressLatch = false;
      }
      break;
    }
    case PpuPort::PpuData: {
      PpuWrite(mVRamAddr.reg, data);
      mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
      break;
    }
  }
}

uint8_t Ppu::CpuRead(uint16_t addr, bool bReadOnly) {
  addr %= PPU_NUM_PORTS;
  uint8_t data = 0x00;
  if (bReadOnly) {
    switch (addr) {
      case PpuPort::Control: {
        data = mControlRegister.reg;
        break;
      }
      case PpuPort::Mask: {
        data = mMaskRegister.reg;
        break;
      }
      case PpuPort::Status: {
        data = mStatusRegister.reg;
        break;
      }
    }
  } else {
    switch (addr) {
      case PpuPort::Status: {
        data = (mStatusRegister.reg & 0xE0) | (mPpuDataBuffer & 0x1F);
        mStatusRegister.verticalBlank = false;
        mAddressLatch = false;
        break;
      }
      case PpuPort::OamData: {
        data = mOamPtr[mOamAddr];
        break;
      }
      case PpuPort::PpuData: {
        data = mPpuDataBuffer;
        mPpuDataBuffer = PpuRead(mVRamAddr.reg);
        if (mVRamAddr.reg >= FRAME_PALETTE_START) {
          data = mPpuDataBuffer;
        }
        mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
        break;
      }
    }
  }
  return data;
}

uint16_t Ppu::DetermineFramePaletteAddress(uint16_t addr) {
  addr %= mFramePalette.size();
  if (addr == 0x0010) {
    addr = 0x0000;
  }
  if (addr == 0x0014) {
    addr = 0x0004;
  }
  if (addr == 0x0018) {
    addr = 0x0008;
  }
  if (addr == 0x001C) {
    addr = 0x000C;
  }
  return addr;
}

void Ppu::PpuWrite(uint16_t addr, uint8_t data) {
  if (PPU_CARTRIDGE_START <= addr && addr <= PPU_CARTRIDGE_END) {
    mCartridge->PpuWrite(addr, data);
  } else if (PPU_NAMETABLE_START <= addr && addr <= PPU_NAMETABLE_END) {
    Mirror(mNameTable, mCartridge->mMirror, addr) = data;
  } else if (FRAME_PALETTE_START <= addr && addr <= FRAME_PALETTE_END) {
    addr = DetermineFramePaletteAddress(addr);
    mFramePalette[addr] = data;
  }
}

uint8_t Ppu::PpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;
  if (PPU_CARTRIDGE_START <= addr && addr <= PPU_CARTRIDGE_END) {
    data = mCartridge->PpuRead(addr);
  } else if (PPU_NAMETABLE_START <= addr && addr <= PPU_NAMETABLE_END) {
    data = Mirror(mNameTable, mCartridge->mMirror, addr);
  } else if (FRAME_PALETTE_START <= addr && addr <= FRAME_PALETTE_END) {
    addr = DetermineFramePaletteAddress(addr);
    data = mFramePalette[addr] & (mMaskRegister.grayscale ? 0x30 : 0x3F);
  }
  return data;
}

void Ppu::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }

void Ppu::Clock() {
  mState->Execute();

  auto xPos = mCycle - 1;
  auto yPos = mScanline;
  auto& color = CalculatePixelColor();
  mRenderer.SetPixelColor(xPos, yPos, color);

  mCycle++;

  if (mCycle >= 341) {
    mCycle = 0;
    mScanline++;
    if (mScanline >= 261) {
      mScanline = -1;
      mRenderer.CommitFrame();
    }
  }
  Transition();
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
  mBg.nextTileId = 0x00;
  mBg.nextTileAttribute = 0x00;
  mBg.nextTileLsb = 0x00;
  mBg.nextTileMsb = 0x00;
  mBg.shifterPatternLow = 0x0000;
  mBg.shifterPatternHigh = 0x0000;
  mBg.shifterAttributeLow = 0x0000;
  mBg.shifterAttributeHigh = 0x0000;
  mStatusRegister.reg = 0x00;
  mMaskRegister.reg = 0x00;
  mControlRegister.reg = 0x00;
  mVRamAddr.reg = 0x0000;
  mTRamAddr.reg = 0x0000;
  mState = &mRenderingState;
}

void Ppu::WritePatternTableToImage(const char* path, uint8_t i, uint8_t palette) {
  const int width = 128;
  const int height = 128;
  const int channels = 3;
  constexpr auto size = width * height * channels;
  std::array<unsigned char, size> image;

  for (uint16_t tileY = 0; tileY < 16; tileY++) {
    for (uint16_t tileX = 0; tileX < 16; tileX++) {
      const uint16_t offset = tileY * 256 + tileX * 16;
      for (uint16_t row = 0; row < 8; row++) {
        auto address = i * 0x1000 + offset + row;
        uint8_t tileLsb = PpuRead(address + 0);
        uint8_t tileMsb = PpuRead(address + 8);

        for (uint16_t col = 0; col < 8; col++) {
          const uint8_t pixel = (tileLsb & 0b01) + (tileMsb & 0b01);
          tileLsb >>= 1;
          tileMsb >>= 1;

          const auto x = tileX * 8 + (7 - col);
          const auto y = tileY * 8 + row;

          const auto index = (y * width + x) * 3;
          const auto& color = GetColorFromPalette(palette, pixel);
          image[index + 0] = static_cast<unsigned char>(color.r);
          image[index + 1] = static_cast<unsigned char>(color.g);
          image[index + 2] = static_cast<unsigned char>(color.b);
        }
      }
    }
  }

  lodepng::encode(path, image.data(), width, height, LCT_RGB);
}

void Ppu::WriteColorPaletteToImage(const char* path) {
  const int numPalettes = 8;
  const int paletteSize = 4;

  std::vector<unsigned char> image;
  for (auto p = 0; p < numPalettes; p++) {
    for (auto s = 0; s < paletteSize; s++) {
      const auto& color = GetColorFromPalette(p, s);
      const auto values = {color.r, color.g, color.b};
      std::copy(values.begin(), values.end(), std::back_inserter(image));
    }
  }

  lodepng::encode(path, image.data(), paletteSize, numPalettes, LCT_RGB);
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
  result.pixel = ReadValueFromBackgroundShifter(mBg.shifterPatternHigh, mBg.shifterPatternLow, bitMux);
  result.palette = ReadValueFromBackgroundShifter(mBg.shifterAttributeHigh, mBg.shifterAttributeLow, bitMux);
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
      mBg.nextTileId = PpuRead(0x2000 | (mVRamAddr.reg & 0x0FFF));
      break;
    }
    case 2: {
      mBg.nextTileAttribute = PpuRead(0x23C0 | (mVRamAddr.nameTableY << 11) | (mVRamAddr.nameTableX << 10) |
                                      ((mVRamAddr.coarseY >> 2) << 3) | (mVRamAddr.coarseX >> 2));

      if (mVRamAddr.coarseY & 0b10) {
        mBg.nextTileAttribute >>= 4;
      }
      if (mVRamAddr.coarseX & 0b10) {
        mBg.nextTileAttribute >>= 2;
      }
      mBg.nextTileAttribute &= 0b11;
      break;
    }
    case 4: {
      auto toReadFrom = (mControlRegister.patternBackground << 12) + (static_cast<uint16_t>(mBg.nextTileId) << 4) +
                        (mVRamAddr.fineY + 0);
      mBg.nextTileLsb = PpuRead(toReadFrom);
      break;
    }
    case 6: {
      auto toReadFrom = (mControlRegister.patternBackground << 12) + (static_cast<uint16_t>(mBg.nextTileId) << 4) +
                        (mVRamAddr.fineY + 8);
      mBg.nextTileMsb = PpuRead(toReadFrom);
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
    mBg.shifterPatternLow <<= 1;
    mBg.shifterPatternHigh <<= 1;

    mBg.shifterAttributeLow <<= 1;
    mBg.shifterAttributeHigh <<= 1;
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
  mBg.shifterPatternLow = (mBg.shifterPatternLow & 0xFF00) | mBg.nextTileLsb;
  mBg.shifterPatternHigh = (mBg.shifterPatternHigh & 0xFF00) | mBg.nextTileMsb;

  mBg.shifterAttributeLow = (mBg.shifterAttributeLow & 0xFF00) | ((mBg.nextTileAttribute & 0b01) ? 0xFF : 0x00);
  mBg.shifterAttributeHigh = (mBg.shifterAttributeHigh & 0xFF00) | ((mBg.nextTileAttribute & 0b10) ? 0xFF : 0x00);
};

void Ppu::Transition() {
  if (mScanline >= 240) {
    mState = &mVerticalBlankState;
  } else if (mCycle >= 256) {
    mState = &mHorizontalBlankState;
  } else {
    mState = &mRenderingState;
  }
}
