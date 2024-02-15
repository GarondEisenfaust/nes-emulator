#pragma once

class Oscilator {
 public:
  Oscilator();
  ~Oscilator();

  double frequency = 0;
  double dutycycle = 0;
  double amplitude = 1;
  int harmonics = 20;

  double Sample(double t);
};
