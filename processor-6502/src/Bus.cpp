#include "Bus.h"
#include "Definitions.h"
#include "Grid.h"

Bus::Bus(RAM* ram) : mRam(ram), mSystemClockCounter(0) {}

Bus::~Bus() {}

void Bus::CpuWrite(uint16_t addr, uint8_t data) {
  if (mCartridge->CpuWrite(addr, data)) {
    return;
  }
  if (RAM_START <= addr && addr <= RAM_END) {
    mRam->at(addr & RAM_SIZE) = data;
    return;
  }
  if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    mPpu->CpuWrite(addr & PPU_RAM_SIZE, data);
    return;
  }
  if (addr >= 0x4016 && addr <= 0x4017) {
    controller_state[addr & 0x0001] = controller[addr & 0x0001];
  }
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly) {
  auto cartridgeResult = mCartridge->CpuRead(addr);
  if (cartridgeResult.hasRead) {
    return cartridgeResult.readResult;
  }
  if (RAM_START <= addr && addr <= RAM_END) {
    return mRam->at(addr & RAM_SIZE);
  }
  if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    return mPpu->CpuRead(addr & PPU_RAM_SIZE, bReadOnly);
  } else if (addr >= 0x4016 && addr <= 0x4017) {
    auto data = (controller_state[addr & 0x0001] & 0x80) > 0;
    controller_state[addr & 0x0001] <<= 1;
    return data;
  }
  return UNDEFINED;
}

void Bus::InsertCartridge(Cartridge* cartridge) {
  mCartridge = cartridge;
  mPpu->InsertCartridge(cartridge);
}

void Bus::Reset() {
  mCpu->Reset();
  mPpu->Reset();
  mCartridge->Reset();
  mSystemClockCounter = 0;
}

void Bus::Clock() {
  mPpu->Clock();
  if (mSystemClockCounter % 3 == 0) {
    mCpu->Clock();
  }

  if (mPpu->nmi) {
    mPpu->nmi = false;
    mCpu->NonMaskableInterrupt();
  }

  mSystemClockCounter++;
}
