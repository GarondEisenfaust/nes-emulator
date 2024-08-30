#pragma once

#include <array>
#include <cmath>
#include <tuple>

template <size_t NUMBER_OF_SAMPLES>
class SineTable {
 public:
  constexpr SineTable() : mSineStep(M_PI / NUMBER_OF_SAMPLES), mSamples(SampleSine()) {}

  float SineLookUp(float x) {
    int timesPi = static_cast<int>(x / M_PI);
    bool negate = timesPi % 2 != 0;
    x -= timesPi * M_PI;

    auto index = static_cast<size_t>(x / mSineStep);
    float result = mSamples[index];

    if (negate) {
      result = -result;
    }

    return result;
  }

  float CosineLookUp(float x) { return SineLookUp(x + (M_PI / 2)); }

 private:
  float mSineStep;
  std::array<float, NUMBER_OF_SAMPLES> mSamples;

  constexpr std::array<float, NUMBER_OF_SAMPLES> SampleSine() {
    double x = 0;
    std::array<float, NUMBER_OF_SAMPLES> samples;

    for (size_t i = 0; i < NUMBER_OF_SAMPLES; i++) {
      samples[i] = std::sin(x);
      x += mSineStep;
    }
    return samples;
  }
};
