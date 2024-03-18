#pragma once

class IAudioOutputDevice {
 public:
  void virtual Write(double data) = 0;
  double virtual Read() = 0;
};
