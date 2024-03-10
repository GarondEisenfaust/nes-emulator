#include "Apu.h"
#include "Bus.h"

Apu::Apu(IAudioOutputDevice& outputDevice)
    : mOutputDevice(outputDevice),
      mPulseChannelOne(false),
      mPulseChannelTwo(true),
      mDmcChannel(this),
      mPulseTable(PrecalculatePulseTable()),
      mTndTable(PrecalculateTndTable()),
      mRunning(false) {}

Apu::~Apu() { Stop(); }

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
    uint8_t interrupt = mInterrupt << 7;
    return interrupt | dmcNotHalted | noiseNotHalted | triangleNotHalted | pulseTimerTwoNotHalted |
           pulseTimerOneNotHalted;
  }
  return 0;
}

uint8_t Apu::ApuRead(uint16_t addr) { return mBus->mCpu->ApuRead(addr); }

void Apu::Clock() {
  mGlobalTime += 6 * (0.3333333333 / 1789773);
  auto quarter = IsQuarterFrameClock(mFrameClockCounter);
  auto half = IsHalfFrameClock(mFrameClockCounter);

  mPulseChannelOne.Clock(quarter, half, mGlobalTime);
  mPulseChannelTwo.Clock(quarter, half, mGlobalTime);
  mNoiseChannel.Clock(quarter, half);
  mDmcChannel.Clock();

  mTriangleChannel.Clock(quarter, half);
  mTriangleChannel.Clock(false, false);

  if (mFrameClockCounter % 20 == 0) {
    output = Mix(mPulseChannelOne.output, mPulseChannelTwo.output, mTriangleChannel.output, mNoiseChannel.output,
                 mDmcChannel.output);
    mOutputDevice.Write(output);
  }
  mFrameClockCounter = ++mFrameClockCounter % onePeriod;
}

void Apu::Reset() {
  for (uint16_t address = APU_RAM_START; address <= APU_RAM_END; address++) {
    CpuWrite(address, 0x00);
  }
  CpuWrite(APU_STATUS, 0x00);
}

void Apu::Start() {
  mRunning = true;
  mApuThread = std::thread([this]() {
    while (mRunning) {
      Clock();
    }
  });
}

void Apu::Stop() {
  mRunning = false;
  mApuThread.join();
}

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

inline float Apu::Mix(double pulseOneOutput, double pulseTwoOutput, uint8_t triangleOutput, uint8_t noiseOutput,
                      uint8_t dmcOutput) {
  auto pulseOut = 0.00752 * (pulseOneOutput + pulseTwoOutput);
  auto tndOut = 0.00851 * triangleOutput + 0.00494 * noiseOutput + 0.00335 * dmcOutput;
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
