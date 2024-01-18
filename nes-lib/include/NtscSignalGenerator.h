#pragma once
#include "PixelColor.h"
#include "SineTable.h"
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

  constexpr static const int mWidth = 256;
  constexpr static const int mHeight = 240;
  constexpr static const int mSamplesToGeneratePerPixel = 8;
  constexpr static const int mSamplesToTakePerPixel = 12;
  constexpr static const int mCyclesForScanline = 341;
  constexpr static const float mHueFix = -3;

  std::array<uint8_t, mWidth * mHeight> mColorBufferNext;

  std::array<float, mWidth * mHeight * mSamplesToGeneratePerPixel> mNtscSignals;
  std::array<YiqData, mWidth * mHeight> mColorBuffer;
  std::array<PixelColor, mWidth * mHeight> mTextureData;
  unsigned int mCurrentSignalIndex = 0;

  SineTable<50000> mSineTable;
};
