#pragma once
#include "PixelColor.h"
#include "SineTable.h"
#include "Surface.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/ShaderProgram.h"
#include "rendering/Texture.h"
#include <array>
#include <cstdint>
#include <memory>

struct YiqData {
  float y = 0;
  float i = 0;
  float q = 0;
};

class NtscSignalFrameDecoder : public IFrameDecoder {
 public:
  NtscSignalFrameDecoder();
  void DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle);

 private:
  Texture mTexture;
  Surface mSurface;
  ShaderProgram mShaderProgram;

  static const size_t mWidth = 256;
  static const size_t mHeight = 240;
  constexpr static const int mSamplesToGeneratePerPixel = 8;
  constexpr static const int mSamplesToTakePerPixel = 12;
  constexpr static const int mCyclesForScanline = 341;
  constexpr static const float mHueFix = 0;

  std::array<float, mWidth * mHeight * mSamplesToGeneratePerPixel> mNtscSignals;
  std::array<PixelColor, mWidth * mHeight> mTextureData;
  SineTable<50000> mSineTable;

  void Draw();

  void GenerateNtscSignal(uint16_t* nesFrameData, unsigned int ppuCycle);
  void GenerateTexture(unsigned int ppuCycle);
  float NtscSignal(int pixel, int phase);
  PixelColor ConvertToRgb(YiqData yiqValue);
};
