#include "LookupTableFrameDecoder.h"
#include <algorithm>

LookupTableFrameDecoder::LookupTableFrameDecoder() : mColorPalette(MakePixelColors()) {}

void LookupTableFrameDecoder::Decode(uint16_t* nesFrameDataBegin, uint16_t* nesFrameDataEnd, unsigned int ppuCycle) {
  std::transform(nesFrameDataBegin, nesFrameDataEnd, mTextureData.begin(),
                 [this](uint16_t pixel) { return this->mColorPalette->at(pixel); });
}

PixelColor* LookupTableFrameDecoder::GetDecodedFrame() { return mTextureData.data(); }
