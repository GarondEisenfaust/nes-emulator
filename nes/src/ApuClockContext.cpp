#include "ApuClockContext.h"
#include "Apu.h"
#include <chrono>

ApuClockContext::ApuClockContext(Apu& apu, IAudioOutputDevice& outputDevice) : mApu(apu), mOutputDevice(outputDevice) {
  Start();
}

ApuClockContext::~ApuClockContext() { Stop(); }

void ApuClockContext::Start() {
  mRunning = true;
  mApuClockThread = std::thread([this]() { Work(); });
}

void ApuClockContext::Stop() {
  mRunning = false;
  mApuClockThread.join();
}

void ApuClockContext::Work() {
  using namespace std::chrono_literals;
  const auto diff = (1000ms / 14834);
  auto next = std::chrono::system_clock::now();
  while (mRunning) {
    double sample = mApu.GenerateNextSample();
    mOutputDevice.Write(sample);
    std::this_thread::sleep_until(next);
    next += diff;
  }
}
