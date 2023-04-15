#include "ppu-states/HorizontalBlankState.h"
#include "Ppu.h"
#include <cstring>

HorizontalBlankState::HorizontalBlankState(Ppu& ppu) : IPpuState(ppu) {}

uint8_t ReverseOrder(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
};

void HorizontalBlankState::Execute() {
  if ((mPpu.mCycle < 258) || (321 <= mPpu.mCycle && mPpu.mCycle < 338)) {
    mPpu.VRamFetch();
  }

  if (mPpu.mCycle == 256) {
    mPpu.IncrementScrollY();
  }

  if (mPpu.mCycle == 257) {
    mPpu.LoadBackgroundShifters();
    mPpu.TransferAddressX();
  }

  if (mPpu.mCycle == 338 || mPpu.mCycle == 340) {
    mPpu.mBackground.nextTileId = mPpu.PpuRead(0x2000 | (mPpu.mVRamAddr.reg & 0x0FFF));
  }

  if (mPpu.mScanline == -1 && 280 <= mPpu.mCycle && mPpu.mCycle < 305) {
    mPpu.TransferAddressY();
  }

  if (mPpu.mCycle == 257 && 0 <= mPpu.mScanline) {
    mPpu.mSpriteOnScanline = {0xFF};
    mPpu.mSpriteCount = 0;

    mPpu.mSpriteShifterPattern.low = {0};
    mPpu.mSpriteShifterPattern.high = {0};
    mPpu.mSpriteZero.onScanline = false;

    for (uint8_t i = 0; i < mPpu.mOam.size(); i++) {
      if (mPpu.mSpriteCount >= mPpu.mSpriteOnScanline.size() + 1) {
        break;
      }
      int16_t diff = mPpu.mScanline - mPpu.mOam[i].y;

      if (!(0 <= diff && diff < (mPpu.mControlRegister.spriteSize ? 16 : 8))) {
        continue;
      }
      if (mPpu.mSpriteCount >= mPpu.mSpriteOnScanline.size()) {
        continue;
      }
      if (i == 0) {
        mPpu.mSpriteZero.onScanline = true;
      }
      mPpu.mSpriteOnScanline[mPpu.mSpriteCount] = mPpu.mOam[i];
      mPpu.mSpriteCount++;
    }
    mPpu.mStatusRegister.spriteOverflow = (mPpu.mSpriteCount > 8);
  }

  if (mPpu.mCycle == 340) {
    for (uint8_t i = 0; i < mPpu.mSpriteCount; i++) {
      struct {
        uint8_t bitsLo;
        uint8_t bitsHi;
        uint16_t addrLo;
        uint16_t addrHi;
      } spritePattern;

      if (!mPpu.mControlRegister.spriteSize) {
        if (!(mPpu.mSpriteOnScanline[i].attribute & (1 << 7))) {
          spritePattern.addrLo = (mPpu.mControlRegister.patternSprite << 12) | (mPpu.mSpriteOnScanline[i].id << 4) |
                                 (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y);

        } else {
          spritePattern.addrLo = (mPpu.mControlRegister.patternSprite << 12) | (mPpu.mSpriteOnScanline[i].id << 4) |
                                 (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y));
        }

      } else {
        if (!(mPpu.mSpriteOnScanline[i].attribute & (1 << 7))) {
          if (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y < 8) {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0b01) << 12) |
                                   ((mPpu.mSpriteOnScanline[i].id & ~0b01) << 4) |
                                   ((mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) % 8);
          } else {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0b01) << 12) |
                                   (((mPpu.mSpriteOnScanline[i].id & ~0b01) + 1) << 4) |
                                   ((mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) % 8);
          }
        } else {
          if (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y < 8) {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0b01) << 12) |
                                   (((mPpu.mSpriteOnScanline[i].id & ~0b01) + 1) << 4) |
                                   (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) % 8);
          } else {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0b01) << 12) |
                                   ((mPpu.mSpriteOnScanline[i].id & ~0b01) << 4) |
                                   (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) % 8);
          }
        }
      }

      spritePattern.addrHi = spritePattern.addrLo + 8;

      spritePattern.bitsLo = mPpu.PpuRead(spritePattern.addrLo);
      spritePattern.bitsHi = mPpu.PpuRead(spritePattern.addrHi);

      if (mPpu.mSpriteOnScanline[i].attribute & 0x40) {
        spritePattern.bitsLo = ReverseOrder(spritePattern.bitsLo);
        spritePattern.bitsHi = ReverseOrder(spritePattern.bitsHi);
      }
      mPpu.mSpriteShifterPattern.low[i] = spritePattern.bitsLo;
      mPpu.mSpriteShifterPattern.high[i] = spritePattern.bitsHi;
    }
  }
}
