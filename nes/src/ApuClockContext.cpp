#include "ApuClockContext.h"
#include "Apu.h"

ApuClockContext::ApuClockContext(Apu& apu, IAudioOutputDevice& outputDevice) : mApu(apu), mOutputDevice(outputDevice) {
  Start();
}

ApuClockContext::~ApuClockContext() { Stop(); }

void ApuClockContext::Start() {
  mRunning = true;
  mApuClockThread = std::thread([this]() {
    while (mRunning) {
      float sample = mApu.GenerateNextSample();
      mOutputDevice.Write(sample);
    }
  });
}

void ApuClockContext::Stop() {
  mRunning = false;
  mApuClockThread.join();
}
