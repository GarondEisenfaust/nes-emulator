#pragma once

class Oscilator {
 public:
  double frequency = 0;
  double dutycycle = 0;
  double amplitude = 1;
  int harmonics = 20;

  Oscilator() = default;
  ~Oscilator() = default;

  double Sample(int time);
};
