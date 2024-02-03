#pragma once
#include "IAudioOutputDevice.h"
#include <memory>

class AudioDevice : public IAudioOutputDevice {
 public:
  AudioDevice();
  ~AudioDevice();
  void Write(float data) override;
  float Read() override;

 private:
  class Impl;
  std::unique_ptr<Impl> mImpl;
};
