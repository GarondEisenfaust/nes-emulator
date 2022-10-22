#include "Bus.h"
#include "Definitions.h"

Bus::Bus() : mSystemClockCounter(0) {
  cpu.ConnectBus(this);

  ram = std::make_unique<RAM>();
}

Bus::~Bus() {}

void Bus::CpuWrite(uint16_t addr, uint8_t data) {
  if (mCartridge->CpuWrite(addr, data)) {
    return;
  }
  if (addr >= RAM_START && addr <= RAM_END) {
    ram->at(addr % RAM_SIZE) = data;
    return;
  }
  if (addr >= PPU_RAM_START && addr <= PPU_RAM_END) {
    ppu.CpuWrite(addr % PPU_RAM_SIZE, data);
    return;
  }
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly) {
  if (mCartridge->CpuRead(addr).hasRead) {
    return UNDEFINED;
  }
  if (addr >= RAM_START && addr <= RAM_END) {
    return ram->at(addr % RAM_SIZE);
  }
  if (addr >= PPU_RAM_START && addr <= PPU_RAM_END) {
    return ppu.CpuRead(addr % PPU_RAM_SIZE, bReadOnly);
  }

  return UNDEFINED;
}

void Bus::InsertCartridge(Cartridge* cartridge) {
  mCartridge = cartridge;
  ppu.InsertCartridge(cartridge);
}

void Bus::Reset() {
  cpu.Reset();
  mSystemClockCounter = 0;
}

void Bus::Clock() {}
