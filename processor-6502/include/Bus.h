#pragma once
#include "Cartridge.h"
#include "Controller.h"
#include "PixelProcessingUnit.h"
#include "Processor6502.h"
#include "RAM.h"
#include <cstdint>
#include <memory>

class Bus {
 public:
  Bus(RAM* ram);
  ~Bus();

  void InsertCartridge(Cartridge* cartridge);
  void Reset();
  void Clock();

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

  void ConnectController(Controller* controller);

  Processor6502* mCpu;
  PixelProcessingUnit* mPpu;

 private:
  Cartridge* mCartridge;
  uint32_t mSystemClockCounter;
  RAM* mRam;
  Controller* mController;
};
