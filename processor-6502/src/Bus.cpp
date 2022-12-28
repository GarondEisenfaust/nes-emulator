#include "Bus.h"
#include "Definitions.h"
#include "Grid.h"

Bus::Bus(RAM* ram) : mRam(ram), mSystemClockCounter(0) {}

Bus::~Bus() {}

void Bus::CpuWrite(uint16_t addr, uint8_t data) {
  if (mCartridge->CpuWrite(addr, data)) {
    return;
  }
  if (addr >= RAM_START && addr <= RAM_END) {
    mRam->at(addr % RAM_SIZE) = data;
    return;
  }
  if (addr >= PPU_RAM_START && addr <= PPU_RAM_END) {
    mPpu->CpuWrite(addr % PPU_RAM_SIZE, data);
    return;
  }
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly) {
  auto cartridgeResult = mCartridge->CpuRead(addr);
  if (cartridgeResult.hasRead) {
    return cartridgeResult.readResult;
  }
  if (addr >= RAM_START && addr <= RAM_END) {
    return mRam->at(addr % RAM_SIZE);
  }
  if (addr >= PPU_RAM_START && addr <= PPU_RAM_END) {
    return mPpu->CpuRead(addr % PPU_RAM_SIZE, bReadOnly);
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
