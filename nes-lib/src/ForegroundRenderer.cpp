#include "ForegroundRenderer.h"
#include "BackgroundRenderer.h"
#include "Ppu.h"

ForegroundPixelInfo ForegroundRenderer::CalculateForegroundPixelInfo() {
  ForegroundPixelInfo result;
  mSpriteZeroBeingRendered = false;

  for (uint8_t i = 0; i < mSpriteCount; i++) {
    if (mSpriteOnScanline[i].x != 0) {
      continue;
    }
    const bool foregroudPixelPixelLow = (mSpriteShifterPattern.low[i] & (1 << 7));
    const bool foregroudPixelPixelHigh = (mSpriteShifterPattern.high[i] & (1 << 7));
    result.pixel = (foregroudPixelPixelHigh << 1) | foregroudPixelPixelLow;

    result.palette = (mSpriteOnScanline[i].attribute & 0b11) + 0x04;
    result.priority = (mSpriteOnScanline[i].attribute & (1 << 5)) == 0;

    if (result.pixel == 0) {
      continue;
    }
    if (i == 0) {
      mSpriteZeroBeingRendered = true;
    }
    break;
  }
  return result;
}

void ForegroundRenderer::UpdateShifters() {
  for (int i = 0; i < mSpriteCount; i++) {
    if (mSpriteOnScanline[i].x > 0) {
      mSpriteOnScanline[i].x--;
      continue;
    }
    mSpriteShifterPattern.low[i] <<= 1;
    mSpriteShifterPattern.high[i] <<= 1;
  }
}

uint8_t ReverseOrder(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
};

void ForegroundRenderer::WriteOamAddress(uint16_t addr, uint8_t data) { mOamAddr = data; }

void ForegroundRenderer::WriteOamData(uint16_t addr, uint8_t data) {
  reinterpret_cast<uint8_t*>(mOam.data())[addr] = data;
}

void ForegroundRenderer::Clock() {
  if (mPpu->mScanline >= 240) {
    VerticalBlankState();
  } else if (mPpu->mCycle >= 256) {
    HorizontalBlankState();
  } else {
    RenderingState();
  }
}

uint8_t ForegroundRenderer::ReadOamData(uint16_t addr) { return reinterpret_cast<uint8_t*>(mOam.data())[addr]; }

void ForegroundRenderer::HorizontalBlankState() {
  if (mPpu->mCycle == 257 && 0 <= mPpu->mScanline) {
    mSpriteOnScanline = {0xFF};
    mSpriteCount = 0;

    mSpriteShifterPattern.low = {0};
    mSpriteShifterPattern.high = {0};
    mSpriteZeroOnScanline = false;

    DetectSpritesOnScanline();
    mPpu->SetSpriteOverflowFlag(mSpriteCount > 8);
  }

  if (mPpu->mCycle == 340) {
    for (uint8_t i = 0; i < mSpriteCount; i++) {
      uint16_t spritePatternAddrLow =
          mPpu->GetSpriteSizeFlag() ? GetLowAddressOfBigSprite(i) : GetLowAddressOfSmallSprite(i);
      LoadSpriteShifters(spritePatternAddrLow, spritePatternAddrLow + 8, i);
    }
  }
}

void ForegroundRenderer::LoadSpriteShifters(uint16_t spritePatternAddrLow, uint16_t spritePatternAddrHigh, uint8_t i) {
  uint8_t spritePatternBitsLow = mPpu->PpuRead(spritePatternAddrLow);
  uint8_t spritePatternBitsHigh = mPpu->PpuRead(spritePatternAddrHigh);

  FlipSpriteIfNecessary(mSpriteOnScanline[i].attribute, spritePatternBitsLow, spritePatternBitsHigh);

  mSpriteShifterPattern.low[i] = spritePatternBitsLow;
  mSpriteShifterPattern.high[i] = spritePatternBitsHigh;
}

uint16_t ForegroundRenderer::GetLowAddressOfBigSprite(uint8_t i) {
  if (!(mSpriteOnScanline[i].attribute & (1 << 7))) {
    if (mPpu->mScanline - mSpriteOnScanline[i].y < 8) {
      return ((mSpriteOnScanline[i].id & 0x01) << 12) | ((mSpriteOnScanline[i].id & 0xFE) << 4) |
             ((mPpu->mScanline - mSpriteOnScanline[i].y) & 0x07);
    } else {
      return ((mSpriteOnScanline[i].id & 0x01) << 12) | (((mSpriteOnScanline[i].id & 0xFE) + 1) << 4) |
             ((mPpu->mScanline - mSpriteOnScanline[i].y) & 0x07);
    }
  } else {
    if (mPpu->mScanline - mSpriteOnScanline[i].y < 8) {
      return ((mSpriteOnScanline[i].id & 0x01) << 12) | (((mSpriteOnScanline[i].id & 0xFE) + 1) << 4) |
             (7 - (mPpu->mScanline - mSpriteOnScanline[i].y) & 0x07);
    } else {
      return ((mSpriteOnScanline[i].id & 0x01) << 12) | ((mSpriteOnScanline[i].id & 0xFE) << 4) |
             (7 - (mPpu->mScanline - mSpriteOnScanline[i].y) & 0x07);
    }
  }
}

uint16_t ForegroundRenderer::GetLowAddressOfSmallSprite(uint8_t i) {
  if (!(mSpriteOnScanline[i].attribute & (1 << 7))) {
    return (mPpu->GetPatternSpriteFlag() << 12) | (mSpriteOnScanline[i].id << 4) |
           (mPpu->mScanline - mSpriteOnScanline[i].y);

  } else {
    return (mPpu->GetPatternSpriteFlag() << 12) | (mSpriteOnScanline[i].id << 4) |
           (7 - (mPpu->mScanline - mSpriteOnScanline[i].y));
  }
}

void ForegroundRenderer::FlipSpriteIfNecessary(uint8_t attribute, uint8_t& spritePatternBitsLow,
                                               uint8_t& spritePatternBitsHigh) {
  if (attribute & 0x40) {
    spritePatternBitsLow = ReverseOrder(spritePatternBitsLow);
    spritePatternBitsHigh = ReverseOrder(spritePatternBitsHigh);
  }
}

void ForegroundRenderer::DetectSpritesOnScanline() {
  for (uint8_t i = 0; i < mOam.size(); i++) {
    if (mSpriteCount >= (mSpriteOnScanline.size() + 1)) {
      break;
    }
    int16_t diff = mPpu->mScanline - mOam[i].y;

    if (!(0 <= diff && diff < (mPpu->GetSpriteSizeFlag() ? 16 : 8))) {
      continue;
    }
    if (mSpriteCount >= mSpriteOnScanline.size()) {
      continue;
    }
    if (i == 0) {
      mSpriteZeroOnScanline = true;
    }
    mSpriteOnScanline[mSpriteCount] = mOam[i];
    mSpriteCount++;
  }
}

void ForegroundRenderer::RenderingState() {
  if (mPpu->mScanline == -1 && mPpu->mCycle == 1) {
    mSpriteShifterPattern.low = {0};
    mSpriteShifterPattern.high = {0};
  }
}

void ForegroundRenderer::VerticalBlankState() {}

void ForegroundRenderer::SetPpu(Ppu* ppu) { mPpu = ppu; }

bool ForegroundRenderer::GetSpriteZeroOnScanlineFlag() { return mSpriteZeroOnScanline; }

bool ForegroundRenderer::GetSpriteZeroBeingRenderedFlag() { return mSpriteZeroBeingRendered; }
