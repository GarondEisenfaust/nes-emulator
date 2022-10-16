#include "Bus.h"
#include "Definitions.h"

Bus::Bus() {
  cpu.ConnectBus(this);

  ram = std::make_unique<RAM>();
}

Bus::~Bus() {}

void Bus::Write(uint16_t addr, uint8_t data) {
  if (addr >= RAM_START && addr <= RAM_END) {
    ram->at(addr % RAM_SIZE) = data;
  }
}

uint8_t Bus::Read(uint16_t addr, bool bReadOnly) {
  if (addr >= RAM_START && addr <= RAM_END) {
    return ram->at(addr % RAM_SIZE);
  }

  return UNDEFINED;
}

void Bus::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }
