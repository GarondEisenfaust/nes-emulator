#pragma once
#include "CircularBuffer.h"
#include "PulseChannel.h"
#include <cstdint>
#include <queue>

class Bus;
class Apu {
 public:
  Apu() = default;
  ~Apu() = default;

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr);
  float GetNextSample();

  void Clock();
  void Reset();

  void ConnectBus(Bus* bus);

 private:
  Bus* mBus;
  int mClockCounter;
  PulseChannel mPulseChannelOne;
  PulseChannel mPulseChannelTwo;
  std::queue<float> queue;
};
