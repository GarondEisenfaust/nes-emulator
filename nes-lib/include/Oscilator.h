#pragma once

class Oscilator {
 public:
  Oscilator();
  ~Oscilator();

  double dutycycle = 0;
  const int harmonics = 20;

  double Sample(double t, double frequency, double amplitude);
};
