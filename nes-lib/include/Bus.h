#pragma once
#include "Cartridge.h"
#include "Cpu.h"
#include "IController.h"
#include "Ppu.h"
#include "Ram.h"
#include <cstdint>
#include <memory>

class Bus {
 public:
  Bus(Ram& ram);
  ~Bus();

  void InsertCartridge(Cartridge* cartridge);
  void Reset();
  void Clock();

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

  void ConnectController(IController* controller);
  void NonMaskableInterrupt();

  Cpu* mCpu;
  Ppu* mPpu;

 private:
  Cartridge* mCartridge;
  uint32_t mClockCounter;
  Ram& mRam;
  IController* mController;
  void Dma(uint8_t page);
};
