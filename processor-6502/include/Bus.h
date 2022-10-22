#pragma once
#include "Cartridge.h"
#include "PixelProcessingUnit.h"
#include "Processor6502.h"
#include "RAM.h"
#include <cstdint>
#include <memory>

class Bus {
 public:
  Bus();
  ~Bus();
  Processor6502 cpu;
  PixelProcessingUnit ppu;

  std::unique_ptr<RAM> ram;

  void InsertCartridge(Cartridge* cartridge);
  void Reset();
  void Clock();

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

 private:
  Cartridge* mCartridge;
  uint32_t mSystemClockCounter;
};
