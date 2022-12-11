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
      mControlRegister.reg = data;
      break;
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
      break;
    case 0x0006:  // PPU Address
      if (mAddressLatch == 0) {
        mPpuAddress = (mPpuAddress & 0x00FF) | (data << 8);
        mAddressLatch = 1;
      } else {
        mPpuAddress = (mPpuAddress & 0xFF00) | data;
        mAddressLatch = 0;
      }
      break;
    case 0x0007:  // PPU Data
      PpuWrite(addr, data);
      mPpuAddress += (mControlRegister.incrementMode ? 32 : 1);
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
      mPpuDataBuffer = PpuRead(mPpuAddress);
      if (mPpuAddress > 0x3F00) {
        data = mPpuDataBuffer;
      }
      mPpuAddress++;
      break;
  }
  return data;
}

void PixelProcessingUnit::PpuWrite(uint16_t addr, uint8_t data) {
  addr &= 0x3FFF;
  if (mCartridge->CpuWrite(addr, data)) {
  } else if (0x0000 <= addr && addr <= 0x1FFF) {
    tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
  } else if (0x2000 <= addr && addr <= 0x3EFF) {
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

  if (mCartridge->CpuRead(addr).hasRead) {
  } else if (0x0000 <= addr && addr <= 0x1FFF) {
    data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
  } else if (0x2000 <= addr && addr <= 0x3EFF) {
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
    data = tblPalette[addr];
  }
  return 0;
}

void PixelProcessingUnit::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }

void PixelProcessingUnit::Clock() {
  if (mScanline == -1 && mCycle == 1) {
    mStatusRegister.verticalBlank = 0;
  }

  if (mScanline == 241 && mCycle == 1) {
    mStatusRegister.verticalBlank = 1;
    if (mControlRegister.enableNmi) {
      nmi = true;
    }
  }

  mGrid->GetPixel(mCycle - 1, mScanline).SetColor(GetColorFromPalette(1, 0));

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

Sprite& PixelProcessingUnit::GetPatternTable(uint8_t i, uint8_t palette) {
  for (auto tileY = 0; tileY < 16; tileY++) {
    for (auto tileX = 0; tileX < 16; tileX++) {
      auto offset = tileY * 256 + tileX * 16;

      for (auto row = 0; row = 8; row++) {
        auto leastSignificant = PpuRead(i * 0x1000 + offset + row + 0);
        auto mostSignificant = PpuRead(i * 0x1000 + offset + row + 8);

        for (auto col = 0; col < 8; col++) {
          uint8_t pixel = (leastSignificant & 0x01) + (mostSignificant & 0x01);
          leastSignificant >>= 1;
          mostSignificant >>= 1;
          auto x = tileX * 8 + (7 - col);
          auto y = tileY * 8 + row;

          mGrid->GetPixel(x, y);
        }
      }
    }
  }
}

PixelColor& PixelProcessingUnit::GetColorFromPalette(uint8_t palette, uint8_t pixel) {
  return mColorPalette->at(PpuRead(0x3F00 + (palette << 2) + pixel));
}
