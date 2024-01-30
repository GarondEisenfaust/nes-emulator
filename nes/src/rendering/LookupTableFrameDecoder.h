#pragma once
#include "ColorPalette.h"
#include "Surface.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/ShaderProgram.h"
#include "rendering/Texture.h"
#include <memory>

class LookupTableFrameDecoder : public IFrameDecoder {
 public:
  LookupTableFrameDecoder();
  void DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle);

 private:
  Texture mTexture;
  Surface mSurface;
  ShaderProgram mShaderProgram;

  static const size_t mTextureWidth = 256;
  static const size_t mTextureHeight = 240;

  std::unique_ptr<ColorPalette> mColorPalette;
  std::array<PixelColor, mTextureWidth * mTextureHeight> mTextureData;
  void Draw();
};
