#include "Bus.h"
#include "Definitions.h"

Bus::Bus() {
  cpu.ConnectBus(this);

  ram = std::make_unique<RAM>();
}

Bus::~Bus() {}

void Bus::Write(uint16_t addr, uint8_t data) {
  if (mCartridge->Write(addr, data)) {
    return;
  }
  if (addr >= RAM_START && addr <= RAM_END) {
    ram->at(addr % RAM_SIZE) = data;
    return;
  }
}

uint8_t Bus::Read(uint16_t addr, bool bReadOnly) {
  if (mCartridge->Read(addr).hasRead) {
    return UNDEFINED;
  }
  if (addr >= RAM_START && addr <= RAM_END) {
    return ram->at(addr % RAM_SIZE);
  }

  return UNDEFINED;
}

void Bus::InsertCartridge(Cartridge* cartridge) { mCartridge = cartridge; }
