#pragma once
#include "IAudioOutputDevice.h"

class AudioDevice : public IAudioOutputDevice {
 public:
  void Write(double data) override;
};
