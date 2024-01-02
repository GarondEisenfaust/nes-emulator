#pragma once
#include "PixelColor.h"
#include <array>
#include <cstdint>

struct YiqData {
  float y;
  float i;
  float q;
};

class NtscSignalGenerator {
 public:
  NtscSignalGenerator() = default;
  ~NtscSignalGenerator() = default;
  float NtscSignal(int pixel, int phase);
  void RenderNtscPixel(unsigned int scanline, unsigned int x, int pixel, int PPU_cycle_counter);
  void ConvertToYiq(float* ntscScanlineSignal, YiqData* yiqScanlineSignal, float phase);
  PixelColor ConvertToRgb(YiqData yiqValue);

  void ConvertYiqToTextureData();
  void SetNtscSignal(unsigned int scanline, unsigned int x, unsigned int offset, float signal);
  float GetNtscSignal(unsigned int scanline, unsigned int x, unsigned int offset);
  void SetYiqPixelValue(unsigned int x, unsigned int y, YiqData value);
  YiqData GetYiqPixelValue(unsigned int x, unsigned int y);

  void SetNextColor(uint8_t color);
  void GenerateTexture(unsigned int ppuCycle);

  static const unsigned int mWidth = 256;
  static const unsigned int mHeight = 240;

  std::array<uint8_t, mWidth * mHeight> mColorBufferNext;

  std::array<float, mWidth * mHeight * 10> mNtscSignals;
  std::array<YiqData, mWidth * mHeight> mColorBuffer;
  std::array<PixelColor, mWidth * mHeight> mTextureData;
  unsigned int mCurrentSignalIndex = 0;
};
