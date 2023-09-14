#pragma once
#include "CircularBuffer.h"
#include "IAudioOutputDevice.h"
#include "NoiseChannel.h"
#include "PulseChannel.h"
#include "TriangleChannel.h"
#include <cstdint>
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

  void ConnectBus(Bus* bus);

 private:
  IAudioOutputDevice& mOutputDevice;
  Bus* mBus;
  int mClockCounter;
  int mFrameClockCounter;
  PulseChannel mPulseChannelOne;
  PulseChannel mPulseChannelTwo;
  NoiseChannel mNoiseChannel;
  TriangleChannel mTriangleChannel;
  std::queue<float> queue;
  float output;

  float minOutputSound = 0;
  float maxOutputSound = 0;

  const int halfFrameClocks[2] = {7416, 14832};
  const int quarterFrameClocks[4] = {3708, halfFrameClocks[0], 11124, halfFrameClocks[1]};

  inline bool IsHalfFrameClock(int clock);
  inline bool IsQuarterFrameClock(int clock);
  inline float Mix(uint8_t pulseOneOutput, uint8_t pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput);
};
