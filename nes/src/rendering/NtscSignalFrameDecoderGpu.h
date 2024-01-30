#pragma once

#include "Surface.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/ShaderProgram.h"
#include "rendering/ShortTexture.h"
#include <array>
#include <cstdint>

struct PixelData {
  uint16_t current = 0;
  uint16_t previous = 0;
  uint16_t cycle = 0;
  uint16_t index = 0;
};

class NtscSignalFrameDecoderGpu : public IFrameDecoder {
 public:
  NtscSignalFrameDecoderGpu();
  void DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle);

 private:
  constexpr static const size_t mTextureWidth = 256;
  constexpr static const size_t mTextureHeight = 240;
  constexpr static const int mSamplesToGeneratePerPixel = 8;
  constexpr static const int mSamplesToTakePerPixel = 12;
  constexpr static const int mCyclesForScanline = 341;
  constexpr static const float mHueFix = 0;

  std::array<PixelData, mTextureWidth * mTextureHeight> mPixels;
  ShortTexture mCurrentPixelTexture;
  Surface mSurface;
  ShaderProgram mShaderProgram;

  void Draw();
};
