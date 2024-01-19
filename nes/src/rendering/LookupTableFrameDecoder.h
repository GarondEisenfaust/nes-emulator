#pragma once
#include "ColorPalette.h"
#include "IFrameDecoder.h"
#include <memory>

class LookupTableFrameDecoder : public IFrameDecoder {
 public:
  LookupTableFrameDecoder();
  void Decode(uint16_t* nesFrameDataBegin, uint16_t* nesFrameDataEnd, unsigned int ppuCycle) override;
  PixelColor* GetDecodedFrame() override;

 private:
  std::unique_ptr<ColorPalette> mColorPalette;
  std::array<PixelColor, 256 * 240> mTextureData;
};
