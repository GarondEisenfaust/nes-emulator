#include "BackgroundRenderer.h"
#include "Ppu.h"

void BackgroundRenderer::Clock() {
  if (mPpu->mScanline >= 240) {
    VerticalBlankState();
  } else if (mPpu->mCycle >= 256) {
    HorizontalBlankState();
  } else {
    RenderingState();
  }
}

void BackgroundRenderer::Reset() {
  mFineX = 0x00;
  mAddressLatch = false;
  mPpuDataBuffer = 0x00;
  nextTileId = 0x00;
  nextTileAttribute = 0x00;
  nextTileLsb = 0x00;
  nextTileMsb = 0x00;
  attributeShifter.Reset();
  patternShifter.Reset();
  mStatusRegister.reg = 0x00;
  mMaskRegister.reg = 0x00;
  mControlRegister.reg = 0x00;
  mVRamAddr.reg = 0x0000;
  mTRamAddr.reg = 0x0000;
}

BackgroundPixelInfo BackgroundRenderer::CalculateBackgroundPixelInfo() {
  BackgroundPixelInfo result;

  if (!mMaskRegister.renderBackground) {
    return result;
  }
  constexpr uint16_t mostSignificantBit = 1 << 15;
  uint16_t bitMux = mostSignificantBit >> mFineX;
  result.pixel = patternShifter.ReadFrom(bitMux);
  result.palette = attributeShifter.ReadFrom(bitMux);
  return result;
}

void BackgroundRenderer::VRamFetch() {
  mPpu->UpdateShifters();
  int stage = (mPpu->mCycle - 1) % 8;
  if (stage == 0) {
    LoadBackgroundShifters();
    FetchNametableTableByte();
  } else if (stage == 2) {
    FetchAttributeTableByte();
  } else if (stage == 4) {
    FetchPatternTableTileLow();
  } else if (stage == 6) {
    FetchPatternTableTileHigh();
  } else if (stage == 7) {
    IncrementScrollX();
  }
}

void BackgroundRenderer::FetchNametableTableByte() { nextTileId = mPpu->PpuRead(0x2000 | (mVRamAddr.reg & 0x0FFF)); }

void BackgroundRenderer::FetchAttributeTableByte() {
  nextTileAttribute = mPpu->PpuRead(0x23C0 | (mVRamAddr.nameTableY << 11) | (mVRamAddr.nameTableX << 10) |
                                    ((mVRamAddr.coarseY >> 2) << 3) | (mVRamAddr.coarseX >> 2));
  if (mVRamAddr.coarseY & 0b10) {
    nextTileAttribute >>= 4;
  }
  if (mVRamAddr.coarseX & 0b10) {
    nextTileAttribute >>= 2;
  }
  nextTileAttribute &= 0b11;
}

void BackgroundRenderer::FetchPatternTableTileHigh() {
  auto toReadFrom = CalculateNextTileIdAddres(8);
  nextTileMsb = mPpu->PpuRead(toReadFrom);
}

void BackgroundRenderer::FetchPatternTableTileLow() {
  auto toReadFrom = CalculateNextTileIdAddres(0);
  nextTileLsb = mPpu->PpuRead(toReadFrom);
}

int BackgroundRenderer::CalculateNextTileIdAddres(uint8_t offset) {
  return (mControlRegister.patternBackground << 12) + (static_cast<uint16_t>(nextTileId) << 4) +
         (mVRamAddr.fineY + offset);
}

void BackgroundRenderer::IncrementScrollY() {
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

void BackgroundRenderer::IncrementScrollX() {
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

void BackgroundRenderer::TransferAddressY() {
  if (!(mMaskRegister.renderBackground || mMaskRegister.renderSprites)) {
    return;
  }
  mVRamAddr.fineY = mTRamAddr.fineY;
  mVRamAddr.nameTableY = mTRamAddr.nameTableY;
  mVRamAddr.coarseY = mTRamAddr.coarseY;
}

void BackgroundRenderer::TransferAddressX() {
  if (!(mMaskRegister.renderBackground || mMaskRegister.renderSprites)) {
    return;
  }
  mVRamAddr.nameTableX = mTRamAddr.nameTableX;
  mVRamAddr.coarseX = mTRamAddr.coarseX;
}

void BackgroundRenderer::UpdateShifters() {
  if (mMaskRegister.renderBackground) {
    patternShifter.Shift();
    attributeShifter.Shift();
  }
}

void BackgroundRenderer::SetPpu(Ppu* ppu) { mPpu = ppu; }

void BackgroundRenderer::LoadBackgroundShifters() {
  patternShifter.Load(nextTileMsb, nextTileLsb);
  attributeShifter.Load((nextTileAttribute & 0b10) ? 0xFF : 0x00, (nextTileAttribute & 0b01) ? 0xFF : 0x00);
};

uint8_t BackgroundRenderer::ReadStatus(uint16_t addr) {
  auto data = (mStatusRegister.reg & 0xE0) | (mPpuDataBuffer & 0x1F);
  mStatusRegister.verticalBlank = false;
  mAddressLatch = false;
  return data;
}

uint8_t BackgroundRenderer::ReadPpuData(uint16_t addr) {
  auto data = mPpuDataBuffer;
  mPpuDataBuffer = mPpu->PpuRead(mVRamAddr.reg);
  if (mVRamAddr.reg >= FRAME_PALETTE_START) {
    data = mPpuDataBuffer;
  }
  mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
  return data;
}

void BackgroundRenderer::WriteControl(uint16_t addr, uint8_t data) {
  mControlRegister.reg = data;
  mTRamAddr.nameTableX = mControlRegister.nametableX;
  mTRamAddr.nameTableY = mControlRegister.nametableY;
}
void BackgroundRenderer::WriteMask(uint16_t addr, uint8_t data) { mMaskRegister.reg = data; }

void BackgroundRenderer::WriteScroll(uint16_t addr, uint8_t data) {
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

void BackgroundRenderer::WritePpuAddress(uint16_t addr, uint8_t data) {
  if (!mAddressLatch) {
    mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0x00FF) | (static_cast<uint16_t>(data & 0b111111) << 8);
    mAddressLatch = true;
  } else {
    mTRamAddr.reg = static_cast<uint16_t>(mTRamAddr.reg & 0xFF00) | static_cast<uint16_t>(data);
    mVRamAddr = mTRamAddr;
    mAddressLatch = false;
  }
}

void BackgroundRenderer::WritePpuData(uint16_t addr, uint8_t data) {
  mPpu->PpuWrite(mVRamAddr.reg, data);
  mVRamAddr.reg += (mControlRegister.incrementMode ? 32 : 1);
}

void BackgroundRenderer::HorizontalBlankState() {
  if ((mPpu->mCycle < 258) || (321 <= mPpu->mCycle && mPpu->mCycle < 338)) {
    VRamFetch();
  }

  if (mPpu->mCycle == 256) {
    IncrementScrollY();
  }

  if (mPpu->mCycle == 257) {
    LoadBackgroundShifters();
    TransferAddressX();
  }

  if (mPpu->mCycle == 338 || mPpu->mCycle == 340) {
    nextTileId = mPpu->PpuRead(0x2000 | (mVRamAddr.reg & 0x0FFF));
  }

  if (mPpu->mScanline == -1 && 280 <= mPpu->mCycle && mPpu->mCycle < 305) {
    TransferAddressY();
  }
}

void BackgroundRenderer::RenderingState() {
  if (mPpu->mScanline == 0 && mPpu->mCycle == 0) {
    mPpu->mCycle = 1;
  }

  if (mPpu->mScanline == -1 && mPpu->mCycle == 1) {
    mStatusRegister.verticalBlank = false;
    mStatusRegister.spriteOverflow = false;
    mStatusRegister.spriteZeroHit = false;
  }

  if (mPpu->mCycle >= 2) {
    VRamFetch();
  }
}

void BackgroundRenderer::VerticalBlankState() {
  if (mPpu->mScanline != 241 || mPpu->mCycle != 1) {
    return;
  }
  mStatusRegister.verticalBlank = true;
  if (mControlRegister.enableNmi) {
    mPpu->mNonMaskableInterrupt = true;
  }
}

bool BackgroundRenderer::GetRenderBackgroundFlag() { return mMaskRegister.renderBackground; }

bool BackgroundRenderer::GetRenderSpritesFlag() { return mMaskRegister.renderSprites; }

bool BackgroundRenderer::GetRenderBackgroundLeftFlag() { return mMaskRegister.renderBackgroundLeft; }

bool BackgroundRenderer::GetRenderSpritesLeftFlag() { return mMaskRegister.renderSpritesLeft; }

bool BackgroundRenderer::GetGrayscale() { return mMaskRegister.grayscale; }

bool BackgroundRenderer::GetSpriteSizeFlag() { return mControlRegister.spriteSize; }

bool BackgroundRenderer::GetPatternSpriteFlag() { return mControlRegister.patternSprite; }

void BackgroundRenderer::SetSpriteOverflowFlag(bool overflow) { mStatusRegister.spriteOverflow = overflow; }

void BackgroundRenderer::SetSpriteZeroHitFlag(bool flag) { mStatusRegister.spriteZeroHit = flag; }
