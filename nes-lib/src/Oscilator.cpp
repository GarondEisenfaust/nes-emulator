#include "Oscilator.h"
#include <math.h>

double Oscilator::Sample(int time) {
  double p = dutycycle * 2.0 * M_PI;
  double first = 0;
  double second = 0;

  for (int n = 1; n < harmonics; n++) {
    auto c = n * frequency * 2.0 * M_PI * time;
    first += std::sin(c) / n;
    second += std::sin(c - p * n) / n;
  }

  return first - second;
}
