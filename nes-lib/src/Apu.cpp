#include "Apu.h"
#include "Bus.h"

Apu::Apu(IAudioOutputDevice& outputDevice)
    : mOutputDevice(outputDevice),
      mPulseChannelOne(false),
      mPulseChannelTwo(true),
      mPulseTable(PrecalculatePulseTable()),
      mTndTable(PrecalculateTndTable()) {}

void Apu::CpuWrite(uint16_t addr, uint8_t data) {
  mPulseChannelOne.Write(addr, data);
  mPulseChannelTwo.Write(addr, data);
  mNoiseChannel.Write(addr, data);
  mTriangleChannel.Write(addr, data);
  mDmcChannel.Write(addr, data);
}

uint8_t Apu::CpuRead(uint16_t addr) {
  if (addr == 0x4015) {
    auto pulseTimerOne = mPulseChannelOne.mSequencer.mDivider.mCounter != 0;
    auto pulseTimerTwo = mPulseChannelTwo.mSequencer.mDivider.mCounter != 0;
    return (pulseTimerTwo << 1) | pulseTimerOne;
  }
  return 0;
}

void Apu::Clock() {
  if (mClockCounter % 6 == 0) {
    auto quarter = IsQuarterFrameClock(mFrameClockCounter);
    auto half = IsHalfFrameClock(mFrameClockCounter);

    mPulseChannelOne.Clock(quarter, half);
    mPulseChannelTwo.Clock(quarter, half);
    mNoiseChannel.Clock(quarter, half);

    for (int i = 0; i < 2; i++) {
      mTriangleChannel.Clock(quarter, half);
    }

    if (mFrameClockCounter % 20 == 0) {
      output = Mix(mPulseChannelOne.output, mPulseChannelTwo.output, mTriangleChannel.output, mNoiseChannel.output);
      mOutputDevice.Write(output);
    }
    mFrameClockCounter = (mFrameClockCounter + 1) % onePeriod;
  }
  mClockCounter++;
}

void Apu::Reset() {}

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}

template <typename T, typename... Opts>
inline bool IsAnyOf(T val, Opts... opts) {
  return (... || (val == opts));
}

bool Apu::IsHalfFrameClock(int clock) { return IsAnyOf(clock, halfFrameClocks[0], halfFrameClocks[1]); }

bool Apu::IsQuarterFrameClock(int clock) {
  return IsAnyOf(clock, quarterFrameClocks[0], quarterFrameClocks[1], quarterFrameClocks[2], quarterFrameClocks[3]);
}

inline float Apu::Mix(uint8_t pulseOneOutput, uint8_t pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput) {
  auto pulseOut = mPulseTable[pulseOneOutput + pulseTwoOutput];
  auto tndOut = mTndTable[3 * triangleOutput + 2 * noiseOutput];

  auto output = pulseOut + tndOut;
  return pulseOut + tndOut;
}

constexpr std::array<double, 31> Apu::PrecalculatePulseTable() {
  std::array<double, 31> table = {0};
  for (int i = 0; i < table.size(); i++) {
    table[i] = 95.52 / (8128.0 / i + 100);
  }
  return table;
}

constexpr std::array<double, 203> Apu::PrecalculateTndTable() {
  std::array<double, 203> table = {0};
  for (int i = 0; i < table.size(); i++) {
    table[i] = 163.67 / (24329.0 / i + 100);
  }
  return table;
}
