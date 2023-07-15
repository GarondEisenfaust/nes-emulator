#pragma once
#include "CircularBuffer.h"
#include "IAudioOutputDevice.h"
#include "PulseChannel.h"
#include <cstdint>
#include <mutex>
#include <queue>

class Bus;
class Apu {
 public:
  Apu(IAudioOutputDevice& outputDevice);
  ~Apu() = default;

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr);

  void Clock();
  void Reset();

  void Lock();
  void Unlock();
  void ConnectBus(Bus* bus);

 private:
  IAudioOutputDevice& mOutputDevice;
  Bus* mBus;
  int mClockCounter;
  PulseChannel mPulseChannelOne;
  PulseChannel mPulseChannelTwo;
  std::queue<float> queue;
  std::mutex queueMutex;
  float output;
};
