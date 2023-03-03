#include "ppu-states/RenderingState.h"
#include "Ppu.h"
#include <cstring>

RenderingState::RenderingState(Ppu& ppu) : IPpuState(ppu) {}

uint8_t FlipByte(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
};

void RenderingState::Execute() {
  if (mPpu.mScanline == 0 && mPpu.mCycle == 0) {
    mPpu.mCycle = 1;
  }

  if (mPpu.mScanline == -1 && mPpu.mCycle == 1) {
    mPpu.mStatusRegister.verticalBlank = 0;
    mPpu.mStatusRegister.spriteOverflow = 0;

    mPpu.mStatusRegister.spriteZeroHit = 0;

    mPpu.mSpriteShifterPatternLo = {0};
    mPpu.mSpriteShifterPatternHi = {0};
  }

  if ((2 <= mPpu.mCycle && mPpu.mCycle < 258) || (321 <= mPpu.mCycle && mPpu.mCycle < 338)) {
    mPpu.VRamFetch();
  }

  if (mPpu.mCycle == 256) {
    IncrementScrollY();
  }

  if (mPpu.mCycle == 257) {
    mPpu.LoadBackgroundShifters();
    TransferAddressX();
  }

  if (mPpu.mCycle == 338 || mPpu.mCycle == 340) {
    mPpu.mBg.nextTileId = mPpu.PpuRead(0x2000 | (mPpu.vRamAddr.reg & 0x0FFF));
  }

  if (mPpu.mScanline == -1 && 280 <= mPpu.mCycle && mPpu.mCycle < 305) {
    TransferAddressY();
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
        spritePattern.bitsLo = FlipByte(spritePattern.bitsLo);
        spritePattern.bitsHi = FlipByte(spritePattern.bitsHi);
      }
      mPpu.mSpriteShifterPatternLo[i] = spritePattern.bitsLo;
      mPpu.mSpriteShifterPatternHi[i] = spritePattern.bitsHi;
    }
  }

  if (!(-1 <= mPpu.mScanline && mPpu.mScanline < 240)) {
    mPpu.Transition<VerticalBlankState>();
  }
}

void RenderingState::IncrementScrollY() {
  if (mPpu.mMaskRegister.renderBackground || mPpu.mMaskRegister.renderSprites) {
    if (mPpu.vRamAddr.fineY < 7) {
      mPpu.vRamAddr.fineY++;
    } else {
      mPpu.vRamAddr.fineY = 0;
      if (mPpu.vRamAddr.coarseY == 29) {
        mPpu.vRamAddr.coarseY = 0;
        mPpu.vRamAddr.nameTableY = !mPpu.vRamAddr.nameTableY;
      } else if (mPpu.vRamAddr.coarseY == 31) {
        mPpu.vRamAddr.coarseY = 0;
      } else {
        mPpu.vRamAddr.coarseY++;
      }
    }
  }
};

void RenderingState::TransferAddressX() {
  if (mPpu.mMaskRegister.renderBackground || mPpu.mMaskRegister.renderSprites) {
    mPpu.vRamAddr.nameTableX = mPpu.tRamAddr.nameTableX;
    mPpu.vRamAddr.coarseX = mPpu.tRamAddr.coarseX;
  }
};

void RenderingState::TransferAddressY() {
  if (mPpu.mMaskRegister.renderBackground || mPpu.mMaskRegister.renderSprites) {
    mPpu.vRamAddr.fineY = mPpu.tRamAddr.fineY;
    mPpu.vRamAddr.nameTableY = mPpu.tRamAddr.nameTableY;
    mPpu.vRamAddr.coarseY = mPpu.tRamAddr.coarseY;
  }
};
