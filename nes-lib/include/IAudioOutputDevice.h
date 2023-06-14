#pragma once

class IAudioOutputDevice {
 public:
  void virtual Write(float data) = 0;
  float virtual Read() = 0;
};
