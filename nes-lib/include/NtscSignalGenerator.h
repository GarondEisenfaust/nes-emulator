#pragma once
#include "PixelColor.h"
#include <array>
#include <cstdint>

struct YiqData {
  float y = -6;
  float i = 0;
  float q = 0;
};

class NtscSignalGenerator {
 public:
  NtscSignalGenerator() = default;
  ~NtscSignalGenerator() = default;
  void SetNextColor(uint8_t color);
  void GenerateNtscSignal(unsigned int ppuCycle);
  void GenerateTexture(unsigned int ppuCycle);
  float NtscSignal(int pixel, int phase);
  PixelColor ConvertToRgb(YiqData yiqValue);

  static const int mWidth = 256;
  static const int mHeight = 240;
  static const int mSamplesToGeneratePerPixel = 8;
  static const int mSamplesToTakePerPixel = 12;
  static const int mCyclesForScanline = 341;

  std::array<uint8_t, mWidth * mHeight> mColorBufferNext;

  std::array<float, mWidth * mHeight * mSamplesToGeneratePerPixel> mNtscSignals;
  std::array<YiqData, mWidth * mHeight> mColorBuffer;
  std::array<PixelColor, mWidth * mHeight> mTextureData;
  unsigned int mCurrentSignalIndex = 0;
};
