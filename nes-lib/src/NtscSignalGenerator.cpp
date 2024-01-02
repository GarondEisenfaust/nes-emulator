#include "NtscSignalGenerator.h"
#include <algorithm>
#include <cmath>
#include <iostream>

float NtscSignalGenerator::NtscSignal(int pixel, int phase) {  // Voltage levels, relative to synch voltage
  static const float black = .518f, white = 1.962f, attenuation = .746f,
                     levels[8] = {.350f,  .518f,  .962f,  1.550f,   // Signal low
                                  1.094f, 1.506f, 1.962f, 1.962f};  // Signal high

  // Decode the NES color.
  int color = (pixel & 0x0F);    // 0..15 "cccc"
  int level = (pixel >> 4) & 3;  // 0..3  "ll"
  int emphasis = (pixel >> 6);   // 0..7  "eee"
  if (color > 13) {
    level = 1;
  }  // For colors 14..15, level 1 is forced.

  // The square wave for this color alternates between these two voltages:
  float low = levels[0 + level];
  float high = levels[4 + level];
  if (color == 0) {
    low = high;
  }  // For color 0, only high level is emitted
  if (color > 12) {
    high = low;
  }  // For colors 13..15, only low level is emitted

  // Generate the square wave
  const auto InColorPhase = [=](int color) { return (color + phase) % 12 < 6; };  // Inline function
  float signal = InColorPhase(color) ? high : low;

  // When de-emphasis bits are set, some parts of the signal are attenuated:
  if (((emphasis & 1) && InColorPhase(0)) || ((emphasis & 2) && InColorPhase(4)) ||
      ((emphasis & 4) && InColorPhase(8))) {
    signal = signal * attenuation;
  }

  return (signal - black) / (white - black);
}

void NtscSignalGenerator::RenderNtscPixel(unsigned int scanline, unsigned int x, int pixel, int PPU_cycle_counter) {
  int phase = PPU_cycle_counter * 8;
  for (int p = 0; p < 8; p++)  // Each pixel produces distinct 8 samples of NTSC signal.
  {
    float signal = NtscSignal(pixel, phase + p);  // Calculated as above
    // Optionally apply some lowpass-filtering to the signal here.
    // Optionally normalize the signal to 0..1 range:
    // printf("%f\n", signal);
    // signal = (signal - 0) / (2 - 0);
    // Save the signal for this pixel.
    SetNtscSignal(scanline, x, p, signal);
  }
}

void NtscSignalGenerator::ConvertToYiq(float* ntscScanlineSignal, YiqData* yiqScanlineSignal, float phase) {
  for (unsigned x = 0; x < mWidth; x++) {
    // Determine the region of scanline signal to sample. Take 12 samples.
    int center = x * (256 * 10) / mWidth + 0;
    int begin = center - 6;
    if (begin < 0) {
      begin = 0;
    }
    int end = center + 6;
    if (end > 256 * 10) {
      end = 256 * 10;
    }
    float y = 0.f, i = 0.f, q = 0.f;   // Calculate the color in YIQ.
    for (int p = begin; p < end; ++p)  // Collect and accumulate samples
    {
      const float level = ntscScanlineSignal[p] / 12.f;
      y += level;
      i += level * std::cos(M_PI * (phase + p) / 6);
      q += level * std::sin(M_PI * (phase + p) / 6);
    }
    yiqScanlineSignal[x] = {y, i, q};  // Send the YIQ color for rendering.
  }
}

PixelColor NtscSignalGenerator::ConvertToRgb(YiqData yiqValue) {
  float gamma = 2.0f;  // Assumed display gamma
  auto gammafix = [=](float f) { return f <= 0.f ? 0.f : std::pow(f, 2.2f / gamma); };
  auto clamp = [](int v) {
    if (v <= 0) {
      return 0;
    }
    return v > 255 ? 255 : v;
  };

  PixelColor rgb{clamp(256 * (yiqValue.y + 0.956f * yiqValue.i + 0.619f * yiqValue.q)),
                 clamp(256 * (yiqValue.y + -0.272f * yiqValue.i + -0.647f * yiqValue.q)),
                 clamp(256 * (yiqValue.y + -1.106f * yiqValue.i + 1.703f * yiqValue.q)), 255};
  // int rgbAsInt = temp;
  // PixelColor rgb = *reinterpret_cast<PixelColor*>(&rgbAsInt);
  // rgb.r = rgb.g;
  // rgb.g = rgb.b;
  // rgb.b = rgb.a;
  // rgb.a = 255;
  // printf("%i %i %i %i\n", rgb.r, rgb.g, rgb.b, rgb.a);
  return rgb;
}

void NtscSignalGenerator::ConvertYiqToTextureData() {
  std::transform(mColorBuffer.begin(), mColorBuffer.end(), mTextureData.begin(),
                 [&](YiqData data) { return ConvertToRgb(data); });
}

void NtscSignalGenerator::SetNtscSignal(unsigned int scanline, unsigned int x, unsigned int offset, float signal) {
  const int index = scanline * mWidth * 10 + x * 10 + offset;
  mNtscSignals[index] = signal;
}

float NtscSignalGenerator::GetNtscSignal(unsigned int scanline, unsigned int x, unsigned int offset) {
  const int index = scanline * mWidth * 10 + x * 10 + offset;
  return mNtscSignals[index];
}

void NtscSignalGenerator::SetYiqPixelValue(unsigned int x, unsigned int y, YiqData value) {
  mColorBuffer[y * mWidth + x] = value;
}

YiqData NtscSignalGenerator::GetYiqPixelValue(unsigned int x, unsigned int y) { return mColorBuffer[y * mWidth + x]; }

void NtscSignalGenerator::SetNextColor(uint8_t color) {
  mColorBufferNext[mCurrentSignalIndex] = color;
  mCurrentSignalIndex = (mCurrentSignalIndex + 1) % mColorBufferNext.size();
}

void NtscSignalGenerator::GenerateTexture(unsigned int ppuCycle) {
  unsigned int scanlinePhase = ppuCycle;
  for (unsigned int y = 0; y < 341; y++) {
    unsigned int takenSamples = 0;
    YiqData yiqData{0, 0, 0};

    for (unsigned int x = 0; x < 261; x++) {
      const unsigned int pixelPhase = ppuCycle * 12;

      if ((0 <= x && x < 256) && (0 <= y && y < 240)) {
        const unsigned int i = y * 256 + x;
        for (unsigned int p = 0; p < 12; p++) {
          const uint8_t pixel = mColorBufferNext[i];
          const float signal = NtscSignal(pixel, pixelPhase + p);
          const float level = signal / 12.f;
          yiqData.y += level;
          yiqData.i += level * std::cos(M_PI * (takenSamples + scanlinePhase) / 6);
          yiqData.q += level * std::sin(M_PI * (takenSamples + scanlinePhase) / 6);

          // printf("%f\n", yiqData.q);

          takenSamples++;
          if (takenSamples % 11 == 0) {
            mTextureData[i] = ConvertToRgb(yiqData);
            yiqData = {0, 0, 0};
          }
        }
      }
      ppuCycle++;
    }
    scanlinePhase = static_cast<int>((scanlinePhase + 341) * 12 + 3.9) % 12;
  }
}
