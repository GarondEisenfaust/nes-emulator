#include "ApproximateSine.h"
#include <cmath>

double ApproximateSine(double t) {
  double j = t * 0.15915;
  j = j - std::floor(j);
  return 20.785 * j * (j - 0.5) * (j - 1.0);
}
