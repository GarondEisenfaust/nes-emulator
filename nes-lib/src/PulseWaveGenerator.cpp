#include "PulseWaveGenerator.h"
#include "ApproximateSine.h"
#include <cmath>
#include <iostream>

double PulseWaveGenerator::Sample(double t, double frequency, double amplitude) {
  double a = 0;
  double b = 0;
  double p = dutycycle * 2.0 * M_PI;
  for (double n = 1; n < harmonics; n++) {
    const double c = n * frequency * 2.0 * M_PI * t;
    a += -ApproximateSine(c) / n;
    b += -ApproximateSine(c - p * n) / n;
  }

  return (2.0 * amplitude / M_PI) * (a - b);
}
