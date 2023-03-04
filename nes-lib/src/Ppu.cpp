#include "Ppu.h"
#include "Bus.h"
#include "Definitions.h"
#include "IRenderer.h"
#include "lodepng.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

Ppu::Ppu(IRenderer& renderer)
    : mCycle(0),
      mScanline(0),
      mColorPalette(std::move(MakePixelColors())),
      mRenderer(renderer),
      nmi(false),
      mRenderingState(*this),
      mVerticalBlankState(*this),
      mHorizontalBlankState(*this),
      mState(&mRenderingState) {}

void Ppu::CpuWrite(uint16_t addr, uint8_t data) {
  switch (addr) {
    case 0x0000:  // Control
    {
      mControlRegister.reg = data;
      mTRamAddr.nameTableX = mControlRegister.nametableX;
      mTRamAddr.nameTableY = mControlRegister.nametableY;
      break;
    }
    case 0x0001:  // Mask
      mMaskRegister.reg = data;
      break;
    case 0x0002:  // Status
      break;
    case 0x0003:  // OAM Address
      mOamAddr = data;
      break;
    case 0x0004:  // OAM Data
      mOamPtr[mOamAddr] = data;
      break;
    case 0x0005:  // Scroll
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
    case 0x0006:  // PPU Address
      if (!mAddressLatch) {
        mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0x00FF) | (static_cast<uint16_t>(data & 0b111111) << 8);
        mAddressLatch = true;
      } else {
        mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0xFF00) | static_cast<uint16_t>(data);
        mVRamAddr = mTRamAddr;
        mAddressLatch = false;
      }
      break;
    case 0x0007:  // PPU Data
      PpuWrite(mVRamAddr.reg, data);
      mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
      break;
  }
}

uint8_t Ppu::CpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;
  if (bReadOnly) {
    switch (addr) {
      case 0x0000: {  // Control
        data = mControlRegister.reg;
        break;
      }
      case 0x0001: {  // Mask
        data = mMaskRegister.reg;
        break;
      }
      case 0x0002: {  // Status
        data = mStatusRegister.reg;
        break;
      }
      case 0x0003: {  // OAM Address
        break;
      }
      case 0x0004: {  // OAM Data
        break;
      }
      case 0x0005: {  // Scroll
        break;
      }
      case 0x0006: {  // PPU Address
        break;
      }
      case 0x0007: {  // PPU Data
        break;
      }
    }
  } else {
    switch (addr) {
      case 0x0000: {  // Control
        break;
      }
      case 0x0001: {  // Mask
        break;
      }
      case 0x0002: {  // Status
        data = (mStatusRegister.reg & 0xE0) | (mPpuDataBuffer & 0x1F);
        mStatusRegister.verticalBlank = false;
        mAddressLatch = false;
        break;
      }
      case 0x0003: {  // OAM Address
        break;
      }
      case 0x0004: {  // OAM Data
        data = mOamPtr[mOamAddr];
        break;
      }
      case 0x0005: {  // Scroll
        break;
      }
      case 0x0006: {  // PPU Address
        break;
      }
      case 0x0007: {  // PPU Data
        data = mPpuDataBuffer;
        mPpuDataBuffer = PpuRead(mVRamAddr.reg);
        if (mVRamAddr.reg >= 0x3F00) {
          data = mPpuDataBuffer;
        }
        mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
        break;
      }
    }
  }
  return data;
}

void Ppu::PpuWrite(uint16_t addr, uint8_t data) {
  addr &= 0x3FFF;
  if (mCartridge->PpuWrite(addr, data)) {
    //
  } else if (0x0000 <= addr && addr <= 0x1FFF) {
    mPatternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
  } else if (0x2000 <= addr && addr <= 0x3EFF) {
    addr &= 0x0FFF;
    if (mCartridge->mMirror == Cartridge::MIRROR::VERTICAL) {
      // Vertical
      if (addr >= 0x0000 && addr <= 0x03FF) {
        mNameTable[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        mNameTable[1][addr & 0x03FF] = data;
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        mNameTable[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        mNameTable[1][addr & 0x03FF] = data;
      }
    } else if (mCartridge->mMirror == Cartridge::MIRROR::HORIZONTAL) {
      // Horizontal
      if (addr >= 0x0000 && addr <= 0x03FF) {
        mNameTable[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        mNameTable[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        mNameTable[1][addr & 0x03FF] = data;
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        mNameTable[1][addr & 0x03FF] = data;
      }
    }
  } else if (0x3F00 <= addr && addr <= 0x3FFF) {
    addr &= 0x001F;
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
    mPaletteTable[addr] = data;
  }
}

uint8_t Ppu::PpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;
  addr &= 0x3FFF;

  if (mCartridge->PpuRead(addr, data)) {
    //
  } else if (0x0000 <= addr && addr <= 0x1FFF) {
    data = mPatternTable[(addr & 0x1000) >> 12][addr & 0x0FFF];
  } else if (0x2000 <= addr && addr <= 0x3EFF) {
    addr &= 0x0FFF;
    if (mCartridge->mMirror == Cartridge::MIRROR::VERTICAL) {
      // Vertical
      if (addr >= 0x0000 && addr <= 0x03FF) {
        data = mNameTable[0][addr & 0x03FF];
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        data = mNameTable[1][addr & 0x03FF];
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        data = mNameTable[0][addr & 0x03FF];
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        data = mNameTable[1][addr & 0x03FF];
      }
    } else if (mCartridge->mMirror == Cartridge::MIRROR::HORIZONTAL) {
      // Horizontal
      if (addr >= 0x0000 && addr <= 0x03FF) {
        data = mNameTable[0][addr & 0x03FF];
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        data = mNameTable[0][addr & 0x03FF];
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        data = mNameTable[1][addr & 0x03FF];
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        data = mNameTable[1][addr & 0x03FF];
      }
    }
  } else if (0x3F00 <= addr && addr <= 0x3FFF) {
    addr &= 0x001F;
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
    data = mPaletteTable[addr] & (mMaskRegister.grayscale ? 0x30 : 0x3F);
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
  auto address = 0x3F00 + (palette << 2) + pixel;
  auto index = PpuRead(address) & 0x3F;
  return mColorPalette->at(index);
}

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

  for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
    for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
      const uint16_t nOffset = nTileY * 256 + nTileX * 16;
      for (uint16_t row = 0; row < 8; row++) {
        uint8_t tile_lsb = PpuRead(i * 0x1000 + nOffset + row + 0x0000);
        uint8_t tile_msb = PpuRead(i * 0x1000 + nOffset + row + 0x0008);

        for (uint16_t col = 0; col < 8; col++) {
          const uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
          tile_lsb >>= 1;
          tile_msb >>= 1;

          const auto x = nTileX * 8 + (7 - col);
          const auto y = nTileY * 8 + row;

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

PixelColor& Ppu::CalculatePixelColor() {
  uint8_t bgPixel = 0x00;
  uint8_t bgPalette = 0x00;

  if (mMaskRegister.renderBackground) {
    uint16_t bitMux = 0x8000 >> mFineX;

    uint8_t p0Pixel = (mBg.shifterPatternLow & bitMux) > 0;
    uint8_t p1Pixel = (mBg.shifterPatternHigh & bitMux) > 0;

    bgPixel = (p1Pixel << 1) | p0Pixel;

    uint8_t bgPal0 = (mBg.shifterAttributeLow & bitMux) > 0;
    uint8_t bgPal1 = (mBg.shifterAttributeHigh & bitMux) > 0;
    bgPalette = (bgPal1 << 1) | bgPal0;
  }

  uint8_t fgPixel = 0x00;
  uint8_t fgPalette = 0x00;
  uint8_t fgPriority = 0x00;

  if (mMaskRegister.renderSprites) {
    mSpriteZeroBeingRendered = false;

    for (uint8_t i = 0; i < mSpriteCount; i++) {
      if (mSpriteOnScanline[i].x == 0) {
        uint8_t fgPixelLo = (mSpriteShifterPatternLo[i] & (1 << 7)) > 0;
        uint8_t fgPixelHi = (mSpriteShifterPatternHi[i] & (1 << 7)) > 0;
        fgPixel = (fgPixelHi << 1) | fgPixelLo;

        fgPalette = (mSpriteOnScanline[i].attribute & 0b11) + 0x04;
        fgPriority = (mSpriteOnScanline[i].attribute & (1 << 5)) == 0;

        if (fgPixel != 0) {
          if (i == 0) {
            mSpriteZeroBeingRendered = true;
          }

          break;
        }
      }
    }
  }

  uint8_t pixel = 0x00;
  uint8_t palette = 0x00;

  if (bgPixel == 0 && fgPixel == 0) {
    pixel = 0x00;
    palette = 0x00;
  } else if (bgPixel == 0 && fgPixel > 0) {
    pixel = fgPixel;
    palette = fgPalette;
  } else if (bgPixel > 0 && fgPixel == 0) {
    pixel = bgPixel;
    palette = bgPalette;
  } else if (bgPixel > 0 && fgPixel > 0) {
    if (fgPriority) {
      pixel = fgPixel;
      palette = fgPalette;
    } else {
      pixel = bgPixel;
      palette = bgPalette;
    }

    if (mSpriteZeroHitPossible && mSpriteZeroBeingRendered) {
      if (mMaskRegister.renderBackground & mMaskRegister.renderSprites) {
        if (~(mMaskRegister.renderBackgroundLeft | mMaskRegister.renderSpritesLeft)) {
          if (mCycle >= 9 && mCycle < 258) {
            mStatusRegister.spriteZeroHit = true;
          }
        } else {
          if (mCycle >= 1 && mCycle < 258) {
            mStatusRegister.spriteZeroHit = true;
          }
        }
      }
    }
  }
  return GetColorFromPalette(palette, pixel);
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
  if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
    if (mVRamAddr.fineY < 7) {
      mVRamAddr.fineY++;
    } else {
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
  }
};

void Ppu::IncrementScrollX() {
  if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
    if (mVRamAddr.coarseX == 31) {
      mVRamAddr.coarseX = 0;
      mVRamAddr.nameTableX = !mVRamAddr.nameTableX;
    } else {
      mVRamAddr.coarseX++;
    }
  }
};

void Ppu::TransferAddressY() {
  if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
    mVRamAddr.fineY = mTRamAddr.fineY;
    mVRamAddr.nameTableY = mTRamAddr.nameTableY;
    mVRamAddr.coarseY = mTRamAddr.coarseY;
  }
};

void Ppu::TransferAddressX() {
  if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
    mVRamAddr.nameTableX = mTRamAddr.nameTableX;
    mVRamAddr.coarseX = mTRamAddr.coarseX;
  }
};

void Ppu::UpdateShifters() {
  if (mMaskRegister.renderBackground) {
    mBg.shifterPatternLow <<= 1;
    mBg.shifterPatternHigh <<= 1;

    mBg.shifterAttributeLow <<= 1;
    mBg.shifterAttributeHigh <<= 1;
  }
  if (mMaskRegister.renderBackground && mCycle >= 1 && mCycle < 258) {
    for (int i = 0; i < mSpriteCount; i++) {
      if (mSpriteOnScanline[i].x > 0) {
        mSpriteOnScanline[i].x--;
      } else {
        mSpriteShifterPatternLo[i] <<= 1;
        mSpriteShifterPatternHi[i] <<= 1;
      }
    }
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
