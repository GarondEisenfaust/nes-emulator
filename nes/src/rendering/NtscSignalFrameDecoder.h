#pragma once
#include "IFrameDecoder.h"
#include "PixelColor.h"
#include "SineTable.h"
#include <array>
#include <cstdint>

struct YiqData {
  float y = 0;
  float i = 0;
  float q = 0;
};

class NtscSignalFrameDecoder : public IFrameDecoder {
 public:
  void Decode(uint16_t* nesFrameDataBegin, uint16_t* nesFrameDataEnd, unsigned int ppuCycle) override;
  PixelColor* GetDecodedFrame() override;

 private:
  void GenerateNtscSignal(uint16_t* nesFrameData, unsigned int ppuCycle);
  void GenerateTexture(unsigned int ppuCycle);
  float NtscSignal(int pixel, int phase);
  PixelColor ConvertToRgb(YiqData yiqValue);

  constexpr static const int mWidth = 256;
  constexpr static const int mHeight = 240;
  constexpr static const int mSamplesToGeneratePerPixel = 8;
  constexpr static const int mSamplesToTakePerPixel = 12;
  constexpr static const int mCyclesForScanline = 341;
  constexpr static const float mHueFix = 0;

  std::array<uint16_t, mWidth * mHeight> mNesFrameBuffer;
  std::array<float, mWidth * mHeight * mSamplesToGeneratePerPixel> mNtscSignals;
  std::array<PixelColor, mWidth * mHeight> mTextureData;

  SineTable<50000> mSineTable;
};
