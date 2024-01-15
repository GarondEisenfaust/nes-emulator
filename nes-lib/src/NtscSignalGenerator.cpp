#include "NtscSignalGenerator.h"
#include <algorithm>
#include <cmath>
#include <iostream>

float NtscSignalGenerator::NtscSignal(int pixel, int phase) {
  // Terminated voltage levels
  static const float black = 0.312f, white = 1.100f,
                     levels[16] = {
                         0.228f, 0.312f, 0.552f, 0.880f,  // Signal low
                         0.616f, 0.840f, 1.100f, 1.100f,  // Signal high
                         0.192f, 0.256f, 0.448f, 0.712f,  // Signal low, attenuated
                         0.500f, 0.676f, 0.896f, 0.896f   // Signal high, attenuated
                     };

  int color = (pixel & 0x0F);
  int level = (pixel >> 4) & 3;
  int emphasis = (pixel >> 6);
  if (color > 13) {
    level = 1;
  }

  auto InColorPhase = [=](int color) { return (color + phase) % 12 < 6; };
  int attenuation = (((emphasis & 1) && InColorPhase(0)) || ((emphasis & 2) && InColorPhase(4)) ||
                     ((emphasis & 4) && InColorPhase(8)) && (color > 0xE))
                        ? 8
                        : 0;

  float low = levels[0 + level] + attenuation;
  float high = levels[4 + level] + attenuation;
  if (color == 0) {
    low = high;
  }  // For color 0, only high level is emitted
  if (color > 12) {
    high = low;
  }
  auto signal = InColorPhase(color) ? high : low;
  return (signal - black) / (white - black);
}

PixelColor NtscSignalGenerator::ConvertToRgb(YiqData yiqValue) {
  float gamma = 2;
  auto gammafix = [=](float f) -> double { return f <= 0.f ? 0.f : std::pow(f, 2.2f / gamma); };

  PixelColor rgb;
  rgb.r = static_cast<uint8_t>(
      std::clamp(255.95 * gammafix(yiqValue.y + 0.946882f * yiqValue.i + 0.623557f * yiqValue.q), 0.0, 255.0));
  rgb.g = static_cast<uint8_t>(
      std::clamp(255.95 * gammafix(yiqValue.y + -0.274788f * yiqValue.i + -0.635691f * yiqValue.q), 0.0, 255.0));
  rgb.b = static_cast<uint8_t>(
      std::clamp(255.95 * gammafix(yiqValue.y + -1.108545f * yiqValue.i + 1.709007f * yiqValue.q), 0.0, 255.0));
  rgb.a = static_cast<uint8_t>(255);

  // printf(" %f %f %f -> %i %i %i %i\n", yiqValue.y, yiqValue.i, yiqValue.q, rgb.r, rgb.g, rgb.b, rgb.a);
  return rgb;
}

void NtscSignalGenerator::SetNextColor(uint8_t color) {
  mColorBufferNext[mCurrentSignalIndex] = color;
  mCurrentSignalIndex = (mCurrentSignalIndex + 1) % mColorBufferNext.size();
}

void NtscSignalGenerator::GenerateNtscSignal(unsigned int ppuCycle) {
  auto colorBufferPointer = mColorBufferNext.begin();

  for (unsigned int y = 0; y < mHeight; y++) {
    ppuCycle = (ppuCycle * mSamplesToGeneratePerPixel) % mSamplesToTakePerPixel;

    for (unsigned int x = 0; x < mWidth; x++) {
      const int firstPixelOfLineIndex = y * mWidth * mSamplesToGeneratePerPixel;
      const int lastPixelOfLineIndex = firstPixelOfLineIndex + mWidth * mSamplesToGeneratePerPixel;
      const int index = firstPixelOfLineIndex + x * mSamplesToGeneratePerPixel;
      const int offset = mSamplesToGeneratePerPixel / 2;
      const int begin = std::max(index - offset, firstPixelOfLineIndex);
      const int end = std::min(index + offset, lastPixelOfLineIndex);

      const auto pixel = *colorBufferPointer;
      for (unsigned int i = begin; i < end; i++) {
        mNtscSignals[i] = NtscSignal(pixel, ppuCycle + i);
      }
      colorBufferPointer++;
    }
    ppuCycle += mCyclesForScanline;
  }
}

void NtscSignalGenerator::GenerateTexture(unsigned int ppuCycle) {
  auto texturePointer = mTextureData.begin();

  for (unsigned int y = 0; y < mHeight; y++) {
    ppuCycle = static_cast<int>(ppuCycle * mSamplesToGeneratePerPixel + 3.9) % mSamplesToTakePerPixel;

    for (unsigned int x = 0; x < mWidth; x++) {
      const int firstPixelOfLineIndex = y * mWidth * mSamplesToGeneratePerPixel;
      const int lastPixelOfLineIndex = firstPixelOfLineIndex + mWidth * mSamplesToGeneratePerPixel;
      const int index = firstPixelOfLineIndex + x * mSamplesToGeneratePerPixel;
      const int offset = mSamplesToTakePerPixel / 2;
      const int begin = std::max(index - offset, firstPixelOfLineIndex);
      const int end = std::min(index + offset, lastPixelOfLineIndex);

      YiqData yiqData{0, 0, 0};
      for (unsigned int i = begin; i < end; i++) {
        const float level = mNtscSignals[i] / mSamplesToTakePerPixel;
        yiqData.y += level;
        yiqData.i += level * std::cos(M_PI * (i + ppuCycle) / offset);
        yiqData.q += level * std::sin(M_PI * (i + ppuCycle) / offset);
      }
      *texturePointer = ConvertToRgb(yiqData);
      texturePointer++;
    }
    ppuCycle += mCyclesForScanline;
  }
}
