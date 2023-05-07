#include "Oscilator.h"
#include <math.h>

double Oscilator::Sample(double time) {
  double p = dutycycle * 2.0 * M_PI;
  double first = 0;
  double second = 0;

  for (int n = 1; n < harmonics; n++) {
    double rightPart = 2 * M_PI * n;
    double leftPart = time * frequency;

    first += std::cos(rightPart * leftPart) / n;
    second += std::cos((rightPart - dutycycle) * leftPart) / n;
  }

  return first - second;
}
