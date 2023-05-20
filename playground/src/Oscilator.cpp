#include "Oscilator.h"
#include <iostream>
#include <math.h>

double ApproximateSin(double x) {
  double t = x * 0.15915;
  t = t - std::floor(t);
  return 20.785 * t * (t - 0.5) * (t - 1.0);
}

double Oscilator::Sample(double time) {
  double f = time - std::floor(time);
  double r;
  // std::cout << std::to_string(f) << "\n";
  if (f < 0.5) {
    r = 1;
  } else {
    r = 0;
  }

  return r;
}
