#pragma once
#include "Cartridge.h"
#include "Cpu.h"
#include "IController.h"
#include "Ppu.h"
#include "RAM.h"
#include <cstdint>
#include <memory>

class Bus {
 public:
  Bus(RAM& ram);
  ~Bus();

  void InsertCartridge(Cartridge* cartridge);
  void Reset();
  void Clock();

  void CpuWrite(uint16_t addr, uint8_t data);
  uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

  void ConnectController(IController* controller);

  Cpu* mCpu;
  Ppu* mPpu;

 private:
  Cartridge* mCartridge;
  uint32_t mSystemClockCounter;
  RAM& mRam;
  IController* mController;

  struct {
    uint8_t page = 0x00;
    uint8_t addr = 0x00;
    uint8_t data = 0x00;
    bool transfer = false;
    bool dummy = false;
  } mDma;
};
