#include "Oscilator.h"
#include <math.h>

double ApproximateSin(double x) {
  double t = x * 0.15915;
  t = t - (int)t;
  return 20.785 * t * (t - 0.5) * (t - 1.0);
}

double Oscilator::Sample(double time) {
  double p = dutycycle * 2.0 * M_PI;
  double first = 0;
  double second = 0;

  for (int n = 1; n < harmonics; n++) {
    auto c = n * frequency * 2.0 * M_PI * time;
    first += ApproximateSin(c) / n;
    second += ApproximateSin(c - p * n) / n;
  }

  return first - second;
}
