#include "Apu.h"
#include "Bus.h"
#include <Definitions.h>
#include <cmath>

Apu::Apu()
    : mApuFrameCounter(*this),
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
  mApuFrameCounter.Write(addr, data);
}

uint8_t Apu::CpuRead(uint16_t addr) {
  if (addr == APU_STATUS) {
    uint8_t pulseTimerOneNotHalted = (!mPulseChannelOne.mLengthCounter.IsHalted()) << 0;
    uint8_t pulseTimerTwoNotHalted = (!mPulseChannelTwo.mLengthCounter.IsHalted()) << 1;
    uint8_t triangleNotHalted = (!mTriangleChannel.mLengthCounter.IsHalted()) << 2;
    uint8_t noiseNotHalted = (!mNoiseChannel.mLengthCounter.IsHalted()) << 3;
    uint8_t dmcNotHalted = (mDmcChannel.mBytesRemaining > 0) << 4;
    uint8_t interrupt = mInterrupt << 7;
    mInterrupt = 0;
    return interrupt | dmcNotHalted | noiseNotHalted | triangleNotHalted | pulseTimerTwoNotHalted |
           pulseTimerOneNotHalted;
  }
  return 0;
}

uint8_t Apu::ApuRead(uint16_t addr) { return mBus->mCpu->ApuRead(addr); }

void Apu::Clock() {
  mApuFrameCounter.Clock();

  mPulseChannelOne.Clock(mApuFrameCounter.mQuarterFrameClock, mApuFrameCounter.mHalfFrameClock, mGlobalTime);
  mPulseChannelTwo.Clock(mApuFrameCounter.mQuarterFrameClock, mApuFrameCounter.mHalfFrameClock, mGlobalTime);
  mNoiseChannel.Clock(mApuFrameCounter.mQuarterFrameClock, mApuFrameCounter.mHalfFrameClock);
  mDmcChannel.Clock();

  mTriangleChannel.Clock(mApuFrameCounter.mQuarterFrameClock, mApuFrameCounter.mHalfFrameClock);
  mTriangleChannel.Clock(false, false);

  UpdateGlobalTime();
}

double Apu::GenerateNextSample() {
  for (int i = 0; i < 20; i++) {
    Clock();
  }
  return Mix(mPulseChannelOne.output, mPulseChannelTwo.output, mTriangleChannel.output, mNoiseChannel.output,
             mDmcChannel.output);
}

void Apu::Reset() {
  for (uint16_t address = APU_RAM_START; address <= APU_RAM_END; address++) {
    CpuWrite(address, 0x00);
  }
  CpuWrite(APU_STATUS, 0x00);
  CpuWrite(APU_FRAME_COUNTER, 0x00);
}

void Apu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mApu = this;
}

inline double Apu::Mix(double pulseOneOutput, double pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput,
                       uint8_t dmcOutput) {
  auto pulseOut = 0.00752 * (pulseOneOutput + pulseTwoOutput);
  auto tndOut = 0.00851 * triangleOutput + 0.00494 * noiseOutput + 0.00335 * dmcOutput;
  return pulseOut + tndOut;
}

void Apu::UpdateGlobalTime() {
  mGlobalTime += 2.0 / CPU_CLOCK_SPEED;

  if (mGlobalTime >= (2 * M_PI)) {
    mGlobalTime = 0;
  }
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
