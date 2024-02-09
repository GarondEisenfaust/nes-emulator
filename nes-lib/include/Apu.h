#pragma once
#include "CircularBuffer.h"
#include "DmcChannel.h"
#include "IAudioOutputDevice.h"
#include "NoiseChannel.h"
#include "PulseChannel.h"
#include "TriangleChannel.h"
#include <cstdint>

#define APU_RAM_START 0x4000
#define APU_RAM_END 0x4013
#define APU_STATUS 0x4015

class Bus;
class Apu {
 public:
  Apu(IAudioOutputDevice& outputDevice);
  ~Apu() = default;

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr);
  uint8_t ApuRead(uint16_t addr);

  void Clock();
  void Reset();

  void ConnectBus(Bus* bus);
  void NonMaskableInterrupt();

 private:
  IAudioOutputDevice& mOutputDevice;
  Bus* mBus;
  int mFrameClockCounter;
  PulseChannel mPulseChannelOne;
  PulseChannel mPulseChannelTwo;
  NoiseChannel mNoiseChannel;
  TriangleChannel mTriangleChannel;
  DmcChannel mDmcChannel;

  std::array<float, 31> mPulseTable;
  std::array<float, 203> mTndTable;
  float output;
  const int halfFrameClocks[2] = {7456, 14914};
  const int quarterFrameClocks[4] = {3728, halfFrameClocks[0], 11185, halfFrameClocks[1]};
  const int onePeriod = 14833;

  inline bool IsHalfFrameClock(int clock);
  inline bool IsQuarterFrameClock(int clock);
  inline float Mix(uint8_t pulseOneOutput, uint8_t pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput,
                   uint8_t dmcOutput);

  constexpr std::array<float, 31> PrecalculatePulseTable();
  constexpr std::array<float, 203> PrecalculateTndTable();
};
