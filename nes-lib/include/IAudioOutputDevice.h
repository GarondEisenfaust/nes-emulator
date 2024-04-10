#pragma once

class IAudioOutputDevice {
 public:
  void virtual Write(double data) = 0;
};
