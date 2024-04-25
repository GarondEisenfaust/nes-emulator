#pragma once
class TriangleWaveGenerator {
 public:
  double dutycycle = 0;
  double Sample(double t, double frequency);

 private:
  const int harmonics = 20;
};
