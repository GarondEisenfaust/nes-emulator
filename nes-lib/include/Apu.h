#pragma once
#include "ApuFrameCounter.h"
#include "DmcChannel.h"
#include "IAudioOutputDevice.h"
#include "NoiseChannel.h"
#include "PulseChannel.h"
#include "TriangleChannel.h"
#include <cstdint>
#include <thread>

#define APU_RAM_START 0x4000
#define APU_RAM_END 0x4013
#define APU_STATUS 0x4015

class Bus;
class Apu {
 public:
  Apu();

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr);
  uint8_t ApuRead(uint16_t addr);

  void Reset();
  void Clock();
  double GenerateNextSample();

  void ConnectBus(Bus* bus);
  bool mInterrupt;
  double mGlobalTime = 0;

 private:
  Bus* mBus;
  ApuFrameCounter mApuFrameCounter;
  PulseChannel mPulseChannelOne;
  PulseChannel mPulseChannelTwo;
  NoiseChannel mNoiseChannel;
  TriangleChannel mTriangleChannel;
  DmcChannel mDmcChannel;

  std::array<float, 31> mPulseTable;
  std::array<float, 203> mTndTable;
  float output;
  const uint32_t onePeriod = 14914;
  const uint32_t halfFrameClocks[2] = {7456, onePeriod};
  const uint32_t quarterFrameClocks[4] = {3728, halfFrameClocks[0], 11185, halfFrameClocks[1]};

  inline bool IsHalfFrameClock(uint32_t clock);
  inline bool IsQuarterFrameClock(uint32_t clock);
  inline double Mix(double pulseOneOutput, double pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput,
                    uint8_t dmcOutput);
  void UpdateGlobalTime();

  constexpr std::array<float, 31> PrecalculatePulseTable();
  constexpr std::array<float, 203> PrecalculateTndTable();
};
