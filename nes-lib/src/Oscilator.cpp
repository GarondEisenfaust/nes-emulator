#include "Oscilator.h"
#include <cmath>
#include <iostream>

Oscilator::Oscilator() {}

Oscilator::~Oscilator() {}

double Oscilator::Sample(double t) {
  double a = 0;
  double b = 0;
  double p = dutycycle * 2.0 * M_PI;
  auto approxsin = [](double t) {
    double j = t * 0.15915;
    j = j - (int)j;
    return 20.785 * j * (j - 0.5) * (j - 1.0);
  };
  for (double n = 1; n < harmonics; n++) {
    double c = n * frequency * 2.0 * M_PI * t;
    a += -approxsin(c) / n;
    b += -approxsin(c - p * n) / n;
  }

  return (2.0 * amplitude / M_PI) * (a - b);
}
