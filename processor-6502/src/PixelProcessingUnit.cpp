#include "PixelProcessingUnit.h"
#include "Bus.h"
#include "Definitions.h"
#include "Grid.h"
#include <cstdlib>

PixelProcessingUnit::PixelProcessingUnit(Grid* grid)
    : mCycle(0),
      mScanline(0),
      mFrameComplete(false),
      mColorPalette(std::move(MakePixelColors())),
      mPatternMemory(std::make_unique<PatternMemory>()),
      mGrid(grid) {}

void PixelProcessingUnit::CpuWrite(uint16_t addr, uint8_t data) {
  switch (addr) {
    case 0x0000:  // Control
    {
      mControlRegister.reg = data;
      tRamAddr.nameTableX = mControlRegister.nametableX;
      tRamAddr.nameTableY = mControlRegister.nametableY;
      break;
    }
    case 0x0001:  // Mask
      mMaskRegister.reg = data;
      break;
    case 0x0002:  // Status
      break;
    case 0x0003:  // OAM Address
      break;
    case 0x0004:  // OAM Data
      break;
    case 0x0005:  // Scroll
      if (mAddressLatch == 0) {
        fineX = data & 0x07;
        tRamAddr.coarseX = data >> 3;
        mAddressLatch = 1;
      } else {
        tRamAddr.fineY = data & 0x07;
        tRamAddr.coarseY = data >> 3;
        mAddressLatch = 0;
      }
      break;
    case 0x0006:  // PPU Address
      if (mAddressLatch == 0) {
        tRamAddr.reg = ((data & 0x3F) << 8) | (tRamAddr.reg & 0x00FF);
        mAddressLatch = 1;
      } else {
        tRamAddr.reg = (tRamAddr.reg & 0xFF00) | data;
        vRamAddr = tRamAddr;
        mAddressLatch = 0;
      }
      break;
    case 0x0007:  // PPU Data
      PpuWrite(vRamAddr.reg, data);
      vRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
      break;
  }
}

uint8_t PixelProcessingUnit::CpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;
  switch (addr) {
    case 0x0000:  // Control
      break;
    case 0x0001:  // Mask
      break;
    case 0x0002:  // Status
      data = (mStatusRegister.reg & 0xE0) | (mPpuDataBuffer & 0x1F);
      mStatusRegister.verticalBlank = 0;
      mAddressLatch = 0;
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
      data = mPpuDataBuffer;
      mPpuDataBuffer = PpuRead(vRamAddr.reg);
      if (vRamAddr.reg > 0x3F00) {
        data = mPpuDataBuffer;
      }
      vRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
      break;
  }
  return data;
}

void PixelProcessingUnit::PpuWrite(uint16_t addr, uint8_t data) {
  addr &= 0x3FFF;
  if (mCartridge->PpuWrite(addr, data)) {
  } else if (0x0000 <= addr && addr <= 0x1FFF) {
    tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
  } else if (0x2000 <= addr && addr <= 0x3EFF) {
    addr &= 0x0FFF;
    if (mCartridge->mMirror == Cartridge::MIRROR::VERTICAL) {
      // Vertical
      if (addr >= 0x0000 && addr <= 0x03FF) {
        tblName[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        tblName[1][addr & 0x03FF] = data;
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        tblName[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        tblName[1][addr & 0x03FF] = data;
      }
    } else if (mCartridge->mMirror == Cartridge::MIRROR::HORIZONTAL) {
      // Horizontal
      if (addr >= 0x0000 && addr <= 0x03FF) {
        tblName[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        tblName[0][addr & 0x03FF] = data;
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        tblName[1][addr & 0x03FF] = data;
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        tblName[1][addr & 0x03FF] = data;
      }
    }
  } else if (0x3F00 <= addr && addr <= 0x3FFF) {
    addr &= 0x001f;
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
    tblPalette[addr] = data;
  }
}

uint8_t PixelProcessingUnit::PpuRead(uint16_t addr, bool bReadOnly) {
  addr &= 0x3FFF;
  uint8_t data = 0x00;
  auto cartridgeResult = mCartridge->PpuRead(addr);
  if (cartridgeResult.hasRead) {
    data = cartridgeResult.readResult;
  } else if (0x0000 <= addr && addr <= 0x1FFF) {
    data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
  } else if (0x2000 <= addr && addr <= 0x3EFF) {
    addr &= 0x0FFF;
    if (mCartridge->mMirror == Cartridge::MIRROR::VERTICAL) {
      // Vertical
      if (addr >= 0x0000 && addr <= 0x03FF) {
        data = tblName[0][addr & 0x03FF];
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        data = tblName[1][addr & 0x03FF];
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        data = tblName[0][addr & 0x03FF];
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        data = tblName[1][addr & 0x03FF];
      }
    } else if (mCartridge->mMirror == Cartridge::MIRROR::HORIZONTAL) {
      // Horizontal
      if (addr >= 0x0000 && addr <= 0x03FF) {
        data = tblName[0][addr & 0x03FF];
      }
      if (addr >= 0x0400 && addr <= 0x07FF) {
        data = tblName[0][addr & 0x03FF];
      }
      if (addr >= 0x0800 && addr <= 0x0BFF) {
        data = tblName[1][addr & 0x03FF];
      }
      if (addr >= 0x0C00 && addr <= 0x0FFF) {
        data = tblName[1][addr & 0x03FF];
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
    data = tblPalette[addr];
  }
  return data;
}

void PixelProcessingUnit::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }

void PixelProcessingUnit::Clock() {
  auto IncrementScrollX = [&]() {
    if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
      if (vRamAddr.coarseX == 31) {
        vRamAddr.coarseX = 0;
        vRamAddr.nameTableX = ~vRamAddr.nameTableX;
      } else {
        vRamAddr.coarseX++;
      }
    }
  };

  auto IncrementScrollY = [&]() {
    if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
      if (vRamAddr.fineY < 7) {
        vRamAddr.fineY++;
      } else {
        vRamAddr.fineY = 0;
        if (vRamAddr.coarseY == 29) {
          vRamAddr.coarseY = 0;
          vRamAddr.nameTableY = ~vRamAddr.nameTableY;
        } else if (vRamAddr.coarseY == 31) {
          vRamAddr.coarseY = 0;
        } else {
          vRamAddr.coarseY++;
        }
      }
    }
  };

  auto TransferAddressX = [&]() {
    if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
      vRamAddr.nameTableX = tRamAddr.nameTableX;
      vRamAddr.coarseX = tRamAddr.coarseX;
    }
  };

  auto TransferAddressY = [&]() {
    if (mMaskRegister.renderBackground || mMaskRegister.renderSprites) {
      vRamAddr.fineY = tRamAddr.fineY;
      vRamAddr.nameTableY = tRamAddr.nameTableY;
      vRamAddr.coarseY = tRamAddr.coarseY;
    }
  };

  auto LoadBackgroundShifters = [&]() {
    mBgShifterPatternLow = (mBgShifterPatternLow & 0xFF00) | mBgNextTileLsb;
    mBgShifterPatternHigh = (mBgShifterPatternHigh & 0xFF00) | mBgNextTileMsb;

    mBgShifterAttributeLow = (mBgShifterAttributeLow & 0xFF00) | ((mBgNextTileAttribute & 0b01) ? 0xFF : 0x00);
    mBgShifterAttributeHigh = (mBgShifterAttributeHigh & 0xFF00) | ((mBgNextTileAttribute & 0b10) ? 0xFF : 0x00);
  };

  auto UpdateShifters = [&]() {
    if (mMaskRegister.renderBackground) {
      mBgShifterPatternLow <<= 1;
      mBgShifterPatternHigh <<= 1;

      mBgShifterAttributeLow <<= 1;
      mBgShifterAttributeHigh <<= 1;
    }
  };

  if (mScanline >= -1 && mCycle < 240) {
    if (mScanline == 0 && mCycle == 0) {
      mCycle = 1;
    }
    if (mScanline == -1 && mCycle == 1) {
      mStatusRegister.verticalBlank = 0;
    }

    if ((mCycle >= 2 && mCycle < 258) || (mCycle >= 321 && mCycle < 338)) {
      UpdateShifters();
      switch ((mCycle - 1) % 8) {
        case 0: {
          LoadBackgroundShifters();
          mBgNextTileId = PpuRead(0x2000 | (vRamAddr.reg & 0x0FFF));
          break;
        }
        case 2: {
          mBgNextTileAttribute = PpuRead(0x23C0 | (vRamAddr.nameTableY << 11) | (vRamAddr.nameTableX << 10) |
                                         ((vRamAddr.coarseY >> 2) << 3) | (vRamAddr.coarseX >> 2));

          if (vRamAddr.coarseY % 0x02) {
            mBgNextTileAttribute >>= 4;
          }
          if (vRamAddr.coarseX % 0x02) {
            mBgNextTileAttribute >>= 2;
          }
          mBgNextTileAttribute &= 0x03;
          break;
        }
        case 4: {
          auto toReadFrom =
              (mControlRegister.patternBackground << 12) + ((uint16_t)mBgNextTileId << 4) + (vRamAddr.fineY + 0);
          mBgNextTileLsb = PpuRead(toReadFrom);
          break;
        }
        case 6: {
          auto toReadFrom =
              (mControlRegister.patternBackground << 12) + ((uint16_t)mBgNextTileId << 4) + (vRamAddr.fineY + 8);
          mBgNextTileMsb = PpuRead(toReadFrom);
          break;
        }
        case 7: {
          IncrementScrollX();
          break;
        }
      }
    }

    if (mCycle == 256) {
      IncrementScrollY();
    }

    if (mCycle == 257) {
      LoadBackgroundShifters();
      TransferAddressX();
    }

    if (mCycle == 338 || mCycle == 340) {
      mBgNextTileId = PpuRead(0x2000 | (vRamAddr.reg & 0x0FFF));
    }

    if (mScanline == -1 && mCycle >= 280 && mCycle < 305) {
      TransferAddressY();
    }
  }

  if (mScanline == 240) {
  }

  if (mScanline == 241 && mCycle == 1) {
    mStatusRegister.verticalBlank = 1;
    if (mControlRegister.enableNmi) {
      nmi = true;
    }
  }

  uint8_t bgPixel = 0x00;
  uint8_t bgPalette = 0x00;
  if (!mMaskRegister.renderBackground) {
    uint16_t bitMux = 0x8000 >> fineX;

    uint8_t p0Pixel = (mBgShifterPatternLow & bitMux) > 0;
    uint8_t p1Pixel = (mBgShifterPatternHigh & bitMux) > 0;

    bgPixel = (p1Pixel << 1) | p0Pixel;

    uint8_t bgPal0 = (mBgShifterAttributeLow & bitMux) > 0;
    uint8_t bgPal1 = (mBgShifterAttributeHigh & bitMux) > 0;
    bgPalette = (bgPal1 << 1) | bgPal0;
  }

  auto& color = GetColorFromPalette(bgPalette, bgPixel);
  mGrid->GetPixel(mCycle - 1, mScanline).SetColor(color);

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

PixelColor& PixelProcessingUnit::GetColorFromPalette(uint8_t palette, uint8_t pixel) {
  auto address = 0x3F00 + (palette << 2) + pixel;
  auto index = PpuRead(address) & 0x3F;
  return mColorPalette->at(index);
}

void PixelProcessingUnit::Reset() {
  fineX = 0x00;
  mAddressLatch = 0x00;
  mScanline = 0;
  mCycle = 0;
  mBgNextTileId = 0x00;
  mBgNextTileAttribute = 0x00;
  mBgNextTileLsb = 0x00;
  mBgNextTileMsb = 0x00;
  mBgShifterPatternLow = 0x0000;
  mBgShifterPatternHigh = 0x0000;
  mBgShifterAttributeLow = 0x0000;
  mBgShifterAttributeHigh = 0x0000;
  mStatusRegister.reg = 0x00;
  mMaskRegister.reg = 0x00;
  mControlRegister.reg = 0x00;
  vRamAddr.reg = 0x0000;
  tRamAddr.reg = 0x0000;
}
