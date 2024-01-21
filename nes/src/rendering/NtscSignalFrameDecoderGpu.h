#pragma once

#include "SingleChannelTexture.h"
#include "Surface.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/ShaderProgram.h"
#include <cstdint>

class NtscSignalFrameDecoderGpu : public IFrameDecoder {
 public:
  NtscSignalFrameDecoderGpu();
  void DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle);

 private:
  SingleChannelTexture mTexture;
  Surface mSurface;
  ShaderProgram mShaderProgram;

  static const size_t mTextureWidth = 256;
  static const size_t mTextureHeight = 240;
  void Draw();
};
