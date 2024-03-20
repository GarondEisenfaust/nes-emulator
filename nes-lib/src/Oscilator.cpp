#include "Oscilator.h"
#include <cmath>
#include <iostream>

Oscilator::Oscilator() {}

Oscilator::~Oscilator() {}

double ApproximateSine(double t) {
  double j = t * 0.15915;
  j = j - std::floor(j);
  return 20.785 * j * (j - 0.5) * (j - 1.0);
}

double Oscilator::Sample(double t, double frequency, double amplitude) {
  double a = 0;
  double b = 0;
  double p = dutycycle * 2.0 * M_PI;
  for (double n = 1; n < harmonics; n++) {
    double c = n * frequency * 2.0 * M_PI * t;
    a += -ApproximateSine(c) / n;
    b += -ApproximateSine(c - p * n) / n;
  }

  return (2.0 * amplitude / M_PI) * (a - b);
}
