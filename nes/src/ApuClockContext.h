#pragma once
#include "IAudioOutputDevice.h"
#include <thread>

class Apu;
class ApuClockContext {
 public:
  ApuClockContext(Apu& apu, IAudioOutputDevice& outputDevice);
  ~ApuClockContext();
  void Start();
  void Stop();

 private:
  std::thread mApuClockThread;
  Apu& mApu;
  IAudioOutputDevice& mOutputDevice;
  bool mRunning;
};
