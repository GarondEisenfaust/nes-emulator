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
    mPpu.mBg.nextTileId = mPpu.PpuRead(0x2000 | (mPpu.vRamAddr.reg & 0x0FFF));
  }

  if (mPpu.mScanline == -1 && 280 <= mPpu.mCycle && mPpu.mCycle < 305) {
    mPpu.TransferAddressY();
  }

  if (mPpu.mCycle == 257 && 0 <= mPpu.mScanline) {
    std::memset(mPpu.mSpriteOnScanline.data(), 0xFF, 8 * sizeof(ObjectAttributeEntry));
    mPpu.mSpriteCount = 0;

    mPpu.mSpriteShifterPatternLo = {0};
    mPpu.mSpriteShifterPatternHi = {0};

    uint8_t nOAMEntry = 0;

    mPpu.bSpriteZeroHitPossible = false;

    while (nOAMEntry < 64 && mPpu.mSpriteCount < 9) {
      int16_t diff = (mPpu.mScanline - static_cast<int16_t>(mPpu.mOam[nOAMEntry].y));

      if (0 <= diff && diff < (mPpu.mControlRegister.spriteSize ? 16 : 8)) {
        if (mPpu.mSpriteCount < 8) {
          if (nOAMEntry == 0) {
            mPpu.bSpriteZeroHitPossible = true;
          }

          std::memcpy(&mPpu.mSpriteOnScanline[mPpu.mSpriteCount], &mPpu.mOam[nOAMEntry], sizeof(ObjectAttributeEntry));
          mPpu.mSpriteCount++;
        }
      }
      nOAMEntry++;
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
        if (!(mPpu.mSpriteOnScanline[i].attribute & 0x80)) {
          spritePattern.addrLo = (mPpu.mControlRegister.patternSprite << 12) | (mPpu.mSpriteOnScanline[i].id << 4) |
                                 (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y);

        } else {
          spritePattern.addrLo = (mPpu.mControlRegister.patternSprite << 12) | (mPpu.mSpriteOnScanline[i].id << 4) |
                                 (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y));
        }

      } else {
        if (!(mPpu.mSpriteOnScanline[i].attribute & 0x80)) {
          if (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y < 8) {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                   ((mPpu.mSpriteOnScanline[i].id & 0xFE) << 4) |
                                   ((mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          } else {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                   (((mPpu.mSpriteOnScanline[i].id & 0xFE) + 1) << 4) |
                                   ((mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          }
        } else {
          if (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y < 8) {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                   (((mPpu.mSpriteOnScanline[i].id & 0xFE) + 1) << 4) |
                                   (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          } else {
            spritePattern.addrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                   ((mPpu.mSpriteOnScanline[i].id & 0xFE) << 4) |
                                   (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
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
      mPpu.mSpriteShifterPatternLo[i] = spritePattern.bitsLo;
      mPpu.mSpriteShifterPatternHi[i] = spritePattern.bitsHi;
    }
  }
}
