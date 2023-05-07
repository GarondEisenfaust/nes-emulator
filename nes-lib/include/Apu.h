#pragma once
#include "CircularBuffer.h"
#include "Oscilator.h"
#include "Sequencer.h"
#include <cstdint>

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
  Sequencer mPulseSequencerOne;
  Sequencer mPulseSequencerTwo;
  Oscilator mPulseOscilatorOne;
  Oscilator mPulseOscilatorTwo;

  CircularBuffer<float, 1024> buffer;
};
