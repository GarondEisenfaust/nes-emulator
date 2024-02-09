#include "Apu.h"
#include "Bus.h"

Apu::Apu(IAudioOutputDevice& outputDevice)
    : mOutputDevice(outputDevice),
      mPulseChannelOne(false),
      mPulseChannelTwo(true),
      mDmcChannel(this),
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
  if (addr == APU_STATUS) {
    uint8_t pulseTimerOneNotHalted = (!mPulseChannelOne.mLengthCounter.IsHalted()) << 0;
    uint8_t pulseTimerTwoNotHalted = (!mPulseChannelTwo.mLengthCounter.IsHalted()) << 1;
    uint8_t triangleNotHalted = (!mTriangleChannel.mLengthCounter.IsHalted()) << 2;
    uint8_t noiseNotHalted = (!mNoiseChannel.mLengthCounter.IsHalted()) << 3;
    uint8_t dmcNotHalted = (mDmcChannel.mBytesRemaining > 0) << 4;
    uint8_t interrupt = (mDmcChannel.mInterupt) << 7;
    return interrupt | dmcNotHalted | noiseNotHalted | triangleNotHalted | pulseTimerTwoNotHalted |
           pulseTimerOneNotHalted;
  }
  return 0;
}

uint8_t Apu::ApuRead(uint16_t addr) { return mBus->mCpu->ApuRead(addr); }

void Apu::Clock() {
  auto quarter = IsQuarterFrameClock(mFrameClockCounter);
  auto half = IsHalfFrameClock(mFrameClockCounter);

  mPulseChannelOne.Clock(quarter, half);
  mPulseChannelTwo.Clock(quarter, half);
  mNoiseChannel.Clock(quarter, half);
  mDmcChannel.Clock();

  for (int i = 0; i < 2; i++) {
    mTriangleChannel.Clock(quarter, half);
  }

  if (mFrameClockCounter % 20 == 0) {
    output = Mix(mPulseChannelOne.output, mPulseChannelTwo.output, mTriangleChannel.output, mNoiseChannel.output,
                 mDmcChannel.output);
    mOutputDevice.Write(output);
  }
  mFrameClockCounter = (mFrameClockCounter + 1) % onePeriod;
}

void Apu::Reset() {}

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}

void Apu::NonMaskableInterrupt() { mBus->mCpu->NonMaskableInterrupt(); }

template <typename T, typename... Opts>
inline bool IsAnyOf(T val, Opts... opts) {
  return (... || (val == opts));
}

bool Apu::IsHalfFrameClock(int clock) { return IsAnyOf(clock, halfFrameClocks[0], halfFrameClocks[1]); }

bool Apu::IsQuarterFrameClock(int clock) {
  return IsAnyOf(clock, quarterFrameClocks[0], quarterFrameClocks[1], quarterFrameClocks[2], quarterFrameClocks[3]);
}
auto i = 0;
inline float Apu::Mix(uint8_t pulseOneOutput, uint8_t pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput,
                      uint8_t dmcOutput) {
  auto pulseOut = mPulseTable[pulseOneOutput + pulseTwoOutput];
  auto tndOut = mTndTable[3 * triangleOutput + 2 * noiseOutput + dmcOutput];
  return pulseOut + tndOut;
}

constexpr std::array<float, 31> Apu::PrecalculatePulseTable() {
  std::array<float, 31> table = {0};
  for (int i = 0; i < table.size(); i++) {
    table[i] = 95.52 / (8128.0 / i + 100);
  }
  return table;
}

constexpr std::array<float, 203> Apu::PrecalculateTndTable() {
  std::array<float, 203> table = {0};
  for (int i = 0; i < table.size(); i++) {
    table[i] = 163.67 / (24329.0 / i + 100);
  }
  return table;
}
