#pragma once

class PulseWaveGenerator {
 public:
  double dutycycle = 0;
  double Sample(double t, double frequency, double amplitude);

 private:
  const int harmonics = 20;
};
