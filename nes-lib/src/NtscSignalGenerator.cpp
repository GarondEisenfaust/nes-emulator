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
  auto InColorPhase = [=](int color) { return (color + phase) % 12 < 6; };  // Inline function
  float signal = InColorPhase(color) ? high : low;

  // When de-emphasis bits are set, some parts of the signal are attenuated:
  if (((emphasis & 1) && InColorPhase(0)) || ((emphasis & 2) && InColorPhase(4)) ||
      ((emphasis & 4) && InColorPhase(8))) {
    signal = signal * attenuation;
  }

  return signal;
}

void NtscSignalGenerator::RenderNtscPixel(unsigned int scanline, unsigned int x, int pixel, int PPU_cycle_counter) {
  int phase = PPU_cycle_counter * 8;
  for (int p = 0; p < 8; p++)  // Each pixel produces distinct 8 samples of NTSC signal.
  {
    float signal = NtscSignal(pixel, phase + p);  // Calculated as above
    // Optionally apply some lowpass-filtering to the signal here.
    // Optionally normalize the signal to 0..1 range:
    static const float black = .518f, white = 1.962f;
    signal = (signal - black) / (white - black);
    // Save the signal for this pixel.
    SetNtscSignal(scanline, x, p, signal);
  }
}

void NtscSignalGenerator::ConvertToYiq(float* ntscScanlineSignal, YiqData* yiqScanlineSignal, float phase) {
  for (unsigned x = 0; x < mWidth; x++) {
    // Determine the region of scanline signal to sample. Take 12 samples.
    int center = x * (256 * 8) / mWidth + 0;
    int begin = center - 6;
    if (begin < 0) {
      begin = 0;
    }
    int end = center + 6;
    if (end > 256 * 8) {
      end = 256 * 8;
    }
    float y = 0.f, i = 0.f, q = 0.f;   // Calculate the color in YIQ.
    for (int p = begin; p < end; ++p)  // Collect and accumulate samples
    {
      float level = ntscScanlineSignal[p] / 12.f;
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

  PixelColor rgb{clamp(255.95 * gammafix(yiqValue.y + 0.946882f * yiqValue.i + 0.623557f * yiqValue.q)),
                 clamp(255.95 * gammafix(yiqValue.y + -0.274788f * yiqValue.i + -0.635691f * yiqValue.q)),
                 clamp(255.95 * gammafix(yiqValue.y + -1.108545f * yiqValue.i + 1.709007f * yiqValue.q)), 255};
  // int rgbAsInt = temp;
  // PixelColor rgb = *reinterpret_cast<PixelColor*>(&rgbAsInt);
  // rgb.r = rgb.g;
  // rgb.g = rgb.b;
  // rgb.b = rgb.a;
  // rgb.a = 255;
  printf("%i %i %i %i\n", rgb.r, rgb.g, rgb.b, rgb.a);
  return rgb;
}

void NtscSignalGenerator::ConvertYiqToTextureData() {
  std::transform(mYiqBuffer.begin(), mYiqBuffer.end(), mTextureData.begin(),
                 [&](YiqData data) { return ConvertToRgb(data); });
}

void NtscSignalGenerator::SetNtscSignal(unsigned int scanline, unsigned int x, unsigned int offset, float signal) {
  int index = scanline * mWidth + x * 8 + offset;
  if (index < 300) {
    printf("Signal %i\n", scanline);
  }
  mNtscSignals[index] = signal;
}

float NtscSignalGenerator::GetNtscSignal(unsigned int scanline, unsigned int x, unsigned int offset) {
  int index = scanline * mWidth + x * 8 + offset;
  return mNtscSignals[index];
}

void NtscSignalGenerator::SetYiqPixelValue(unsigned int x, unsigned int y, YiqData value) {
  mYiqBuffer[y * mWidth + x] = value;
}

YiqData NtscSignalGenerator::GetYiqPixelValue(unsigned int x, unsigned int y) { return mYiqBuffer[y * mWidth + x]; }
