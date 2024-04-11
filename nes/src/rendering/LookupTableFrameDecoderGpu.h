#pragma once
#include "ColorPalette.h"
#include "Surface.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/ShaderProgram.h"
#include "rendering/SingleChannelShortTexture.h"
#include <memory>

class LookupTableFrameDecoderGpu : public IFrameDecoder {
 public:
  LookupTableFrameDecoderGpu();
  void DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle);

 private:
  SingleChannelShortTexture mTexture;
  Surface mSurface;
  ShaderProgram mShaderProgram;

  static const size_t mTextureWidth = 256;
  static const size_t mTextureHeight = 240;

  std::unique_ptr<ColorPaletteF> mColorPalette;
  std::array<PixelColorU8, mTextureWidth * mTextureHeight> mTextureData;
  void Draw();
};
