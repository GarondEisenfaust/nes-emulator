#include "ppu-states/VisibleScreenSpaceState.h"
#include "Ppu.h"
#include <cstring>

VisibleScreenSpaceState::VisibleScreenSpaceState(Ppu& ppu) : IPpuState(ppu) {}

uint8_t FlipByte(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
};

void VisibleScreenSpaceState::Execute() {
  if (mPpu.mScanline == 0 && mPpu.mCycle == 0) {
    mPpu.mCycle = 1;
  }
  if (mPpu.mScanline == -1 && mPpu.mCycle == 1) {
    mPpu.mStatusRegister.verticalBlank = 0;
    mPpu.mStatusRegister.spriteOverflow = 0;

    mPpu.mStatusRegister.spriteZeroHit = 0;

    for (int i = 0; i < 8; i++) {
      mPpu.mSpriteShifterPatternLo[i] = 0;
      mPpu.mSpriteShifterPatternHi[i] = 0;
    }
  }

  if ((mPpu.mCycle >= 2 && mPpu.mCycle < 258) || (mPpu.mCycle >= 321 && mPpu.mCycle < 338)) {
    mPpu.UpdateShifters();
    switch ((mPpu.mCycle - 1) % 8) {
      case 0: {
        mPpu.LoadBackgroundShifters();
        mPpu.mBgNextTileId = mPpu.PpuRead(0x2000 | (mPpu.vRamAddr.reg & 0x0FFF));
        break;
      }
      case 2: {
        mPpu.mBgNextTileAttribute =
            mPpu.PpuRead(0x23C0 | (mPpu.vRamAddr.nameTableY << 11) | (mPpu.vRamAddr.nameTableX << 10) |
                         ((mPpu.vRamAddr.coarseY >> 2) << 3) | (mPpu.vRamAddr.coarseX >> 2));

        if (mPpu.vRamAddr.coarseY & 0x02) {
          mPpu.mBgNextTileAttribute >>= 4;
        }
        if (mPpu.vRamAddr.coarseX & 0x02) {
          mPpu.mBgNextTileAttribute >>= 2;
        }
        mPpu.mBgNextTileAttribute &= 0x03;
        break;
      }
      case 4: {
        auto toReadFrom = (mPpu.mControlRegister.patternBackground << 12) +
                          (static_cast<uint16_t>(mPpu.mBgNextTileId) << 4) + (mPpu.vRamAddr.fineY + 0);
        mPpu.mBgNextTileLsb = mPpu.PpuRead(toReadFrom);
        break;
      }
      case 6: {
        auto toReadFrom = (mPpu.mControlRegister.patternBackground << 12) +
                          (static_cast<uint16_t>(mPpu.mBgNextTileId) << 4) + (mPpu.vRamAddr.fineY + 8);
        mPpu.mBgNextTileMsb = mPpu.PpuRead(toReadFrom);
        break;
      }
      case 7: {
        mPpu.IncrementScrollX();
        break;
      }
    }
  }

  if (mPpu.mCycle == 256) {
    mPpu.IncrementScrollY();
  }

  if (mPpu.mCycle == 257) {
    mPpu.LoadBackgroundShifters();
    mPpu.TransferAddressX();
  }

  if (mPpu.mCycle == 338 || mPpu.mCycle == 340) {
    mPpu.mBgNextTileId = mPpu.PpuRead(0x2000 | (mPpu.vRamAddr.reg & 0x0FFF));
  }

  if (mPpu.mScanline == -1 && mPpu.mCycle >= 280 && mPpu.mCycle < 305) {
    mPpu.TransferAddressY();
  }
  if (mPpu.mCycle == 257 && mPpu.mScanline >= 0) {
    std::memset(mPpu.mSpriteOnScanline.data(), 0xFF, 8 * sizeof(ObjectAttributeEntry));
    mPpu.mSpriteCount = 0;

    for (uint8_t i = 0; i < 8; i++) {
      mPpu.mSpriteShifterPatternLo[i] = 0;
      mPpu.mSpriteShifterPatternHi[i] = 0;
    }
    uint8_t nOAMEntry = 0;

    mPpu.bSpriteZeroHitPossible = false;

    while (nOAMEntry < 64 && mPpu.mSpriteCount < 9) {
      int16_t diff = ((int16_t)mPpu.mScanline - (int16_t)mPpu.mOam[nOAMEntry].y);

      if (diff >= 0 && diff < (mPpu.mControlRegister.spriteSize ? 16 : 8)) {
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
      uint8_t spritePatternBitsLo;
      uint8_t spritePatternBitsHi;
      uint16_t spritePatternAddrLo;
      uint16_t spritePatternAddrHi;

      if (!mPpu.mControlRegister.spriteSize) {
        if (!(mPpu.mSpriteOnScanline[i].attribute & 0x80)) {
          spritePatternAddrLo = (mPpu.mControlRegister.patternSprite << 12) | (mPpu.mSpriteOnScanline[i].id << 4) |
                                (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y);

        } else {
          spritePatternAddrLo = (mPpu.mControlRegister.patternSprite << 12) | (mPpu.mSpriteOnScanline[i].id << 4) |
                                (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y));
        }

      } else {
        if (!(mPpu.mSpriteOnScanline[i].attribute & 0x80)) {
          if (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y < 8) {
            spritePatternAddrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                  ((mPpu.mSpriteOnScanline[i].id & 0xFE) << 4) |
                                  ((mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          } else {
            spritePatternAddrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                  (((mPpu.mSpriteOnScanline[i].id & 0xFE) + 1) << 4) |
                                  ((mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          }
        } else {
          if (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y < 8) {
            spritePatternAddrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                  (((mPpu.mSpriteOnScanline[i].id & 0xFE) + 1) << 4) |
                                  (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          } else {
            spritePatternAddrLo = ((mPpu.mSpriteOnScanline[i].id & 0x01) << 12) |
                                  ((mPpu.mSpriteOnScanline[i].id & 0xFE) << 4) |
                                  (7 - (mPpu.mScanline - mPpu.mSpriteOnScanline[i].y) & 0x07);
          }
        }
      }

      spritePatternAddrHi = spritePatternAddrLo + 8;

      spritePatternBitsLo = mPpu.PpuRead(spritePatternAddrLo);
      spritePatternBitsHi = mPpu.PpuRead(spritePatternAddrHi);

      if (mPpu.mSpriteOnScanline[i].attribute & 0x40) {
        spritePatternBitsLo = FlipByte(spritePatternBitsLo);
        spritePatternBitsHi = FlipByte(spritePatternBitsHi);
      }
      mPpu.mSpriteShifterPatternLo[i] = spritePatternBitsLo;
      mPpu.mSpriteShifterPatternHi[i] = spritePatternBitsHi;
    }
  }

  if (mPpu.mScanline == 241 && mPpu.mCycle == 1) {
    mPpu.Transition<VerticalBlankState>();
  } else if (!(mPpu.mScanline >= -1 && mPpu.mScanline < 240)) {
    mPpu.Transition<NotVisibleScreenSpaceState>();
  }
}
