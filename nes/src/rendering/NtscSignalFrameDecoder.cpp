#include <glad/glad.h>
#include "NtscSignalFrameDecoder.h"
#include "DisplayTextureShader.h"
#include "VertexShader.h"
#include <algorithm>
#include <cmath>
#include <iostream>

NtscSignalFrameDecoder::NtscSignalFrameDecoder() : mTexture(mWidth, mHeight, GL_NEAREST) {
  Shader fragmentShader(DisplayTextureShader().source, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShader().source, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
  mShaderProgram.Use();
}

float NtscSignalFrameDecoder::NtscSignal(int pixel, int phase) {
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

  auto InColorPhase = [=](int color) {
    return (color + phase + mSamplesToGeneratePerPixel) % mSamplesToTakePerPixel < 6;
  };
  int attenuation = (((emphasis & 1) && InColorPhase(0)) || ((emphasis & 2) && InColorPhase(4)) ||
                     ((emphasis & 4) && InColorPhase(8)) && (color > 0xE))
                        ? 8
                        : 0;

  float low = levels[0 + level] + attenuation;
  float high = levels[4 + level] + attenuation;
  if (color == 0) {
    low = high;
  }
  if (color > 12) {
    high = low;
  }
  auto signal = InColorPhase(color) ? high : low;
  return (signal - black) / (white - black);
}

PixelColorU8 NtscSignalFrameDecoder::ConvertToRgb(YiqData yiqValue) {
  float gamma = 2;
  auto gammafix = [=](float f) -> double { return f <= 0.f ? 0.f : std::pow(f, 2.2f / gamma); };

  PixelColorU8 rgb;
  rgb.r = static_cast<uint8_t>(
      std::clamp(255.0 * (yiqValue.y + 0.946882f * yiqValue.i + 0.623557f * yiqValue.q), 0.0, 255.0));
  rgb.g = static_cast<uint8_t>(
      std::clamp(255.0 * (yiqValue.y + -0.274788f * yiqValue.i + -0.635691f * yiqValue.q), 0.0, 255.0));
  rgb.b = static_cast<uint8_t>(
      std::clamp(255.0 * (yiqValue.y + -1.108545f * yiqValue.i + 1.709007f * yiqValue.q), 0.0, 255.0));
  rgb.a = static_cast<uint8_t>(255);

  return rgb;
}

void NtscSignalFrameDecoder::DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle) {
  GenerateNtscSignal(frameData, ppuCycle);
  GenerateTexture(ppuCycle);
  mTexture.UpdateData(reinterpret_cast<uint8_t*>(mTextureData.data()));
  Draw();
}

void NtscSignalFrameDecoder::Draw() {
  mTexture.Bind();
  mSurface.Draw();
}

void NtscSignalFrameDecoder::GenerateNtscSignal(uint16_t* nesFrameData, unsigned int ppuCycle) {
  for (unsigned int y = 0; y < mHeight; y++) {
    ppuCycle = ppuCycle % mSamplesToTakePerPixel * mSamplesToGeneratePerPixel;

    for (unsigned int x = 0; x < mWidth; x++) {
      const int firstPixelOfLineIndex = y * mWidth * mSamplesToGeneratePerPixel;
      const int lastPixelOfLineIndex = firstPixelOfLineIndex + mWidth * mSamplesToGeneratePerPixel;
      const int index = firstPixelOfLineIndex + x * mSamplesToGeneratePerPixel;
      const int offset = mSamplesToGeneratePerPixel / 2;
      const int begin = std::max(index - offset, firstPixelOfLineIndex);
      const int end = std::min(index + offset, lastPixelOfLineIndex);

      const auto pixel = *nesFrameData;
      for (unsigned int i = begin; i < end; i++) {
        mNtscSignals[i] = NtscSignal(pixel, ppuCycle + i);
      }
      nesFrameData++;
    }
    ppuCycle += mCyclesForScanline;
  }
}

void NtscSignalFrameDecoder::GenerateTexture(unsigned int ppuCycle) {
  auto texturePointer = mTextureData.begin();

  for (unsigned int y = 0; y < mHeight; y++) {
    ppuCycle = ppuCycle % mSamplesToTakePerPixel * mSamplesToGeneratePerPixel;

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
        yiqData.i += level * mSineTable.CosineLookUp((M_PI / offset) * (ppuCycle + i + mHueFix));
        yiqData.q += level * mSineTable.SineLookUp((M_PI / offset) * (ppuCycle + i + mHueFix));
      }
      *texturePointer = ConvertToRgb(yiqData);
      texturePointer++;
    }
    ppuCycle += mCyclesForScanline;
  }
}
