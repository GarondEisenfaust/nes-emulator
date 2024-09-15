#pragma once
#include "ColorPalette.h"
#include "IFrameDecoder.h"
#include "ShapeRendering/ShaderProgram.h"
#include "Surface.h"
#include "Texture.h"
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

  std::unique_ptr<ColorPaletteU8> mColorPalette;
  std::array<PixelColorU8, mTextureWidth * mTextureHeight> mTextureData;
  void Draw();
};
