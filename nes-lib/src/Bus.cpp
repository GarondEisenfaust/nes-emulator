#include "Bus.h"
#include "Definitions.h"
#include "IRenderer.h"

Bus::Bus(Ram& ram) : mRam(ram), mClockCounter(0) {}

Bus::~Bus() {}

void Bus::CpuWrite(uint16_t addr, uint8_t data) {
  if (mCartridge->CpuWrite(addr, data)) {
  } else if (RAM_START <= addr && addr <= RAM_END) {
    mRam.Write(addr, data);
  } else if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    mPpu->CpuWrite(addr, data);
  } else if (addr == 0x4014) {
    Dma(data);
  } else if (0x4016 <= addr && addr <= 0x4017) {
    mController->Write(addr);
  }
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;

  if (mCartridge->CpuRead(addr, data)) {
  } else if (RAM_START <= addr && addr <= RAM_END) {
    data = mRam.Read(addr);
  } else if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    data = mPpu->CpuRead(addr, bReadOnly);
  } else if (0x4016 <= addr && addr <= 0x4017) {
    data = mController->Read(addr);
  }
  return data;
}

void Bus::ConnectController(IController* controller) { mController = controller; }

void Bus::InsertCartridge(Cartridge* cartridge) {
  mCartridge = cartridge;
  mPpu->InsertCartridge(cartridge);
}

void Bus::Reset() {
  mCartridge->Reset();
  mCpu->Reset();
  mPpu->Reset();
  mClockCounter = 0;
}

void Bus::Clock() {
  mPpu->Clock();
  if (mClockCounter % 3 == 0) {
    mCpu->Clock();
  }

  if (mPpu->nmi) {
    mPpu->nmi = false;
    mCpu->NonMaskableInterrupt();
  }

  mClockCounter++;
}

void Bus::Dma(uint8_t page) {
  uint16_t address = page << 8;

  for (auto i = 0; i < 256; i++) {
    mPpu->mOamPtr[i] = CpuRead(address);
    address++;
  }

  mCpu->cycles += 513 + (mClockCounter % 2);
}
