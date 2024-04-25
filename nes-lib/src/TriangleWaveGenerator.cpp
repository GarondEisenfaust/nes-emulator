#include "TriangleWaveGenerator.h"
#include "ApproximateSine.h"
#include <cmath>
#include <iostream>

double TriangleWaveGenerator::Sample(double t, double frequency) {
  return 15.0 * ApproximateSine(2 * M_PI * frequency * t);
}
