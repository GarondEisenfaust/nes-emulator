#include "Bus.h"
#include "Definitions.h"
#include "IRenderer.h"

#define DMA_ADDRESS 0x4014

Bus::Bus(Ram& ram) : mRam(ram), mClockCounter(0) {}

Bus::~Bus() {}

void Bus::CpuWrite(uint16_t addr, uint8_t data) {
  if (RAM_START <= addr && addr <= RAM_END) {
    mRam.Write(addr, data);
  } else if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    mPpu->CpuWrite(addr, data);
  } else if (addr == DMA_ADDRESS) {
    Dma(data);
  } else if (CONTROLLER_START <= addr && addr <= CONTROLLER_END) {
    mController->Write(addr);
  } else if (CPU_CARTRIDGE_START <= addr && addr <= CPU_CARTRIDGE_END) {
    mCartridge->CpuWrite(addr, data);
  }
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;

  if (RAM_START <= addr && addr <= RAM_END) {
    data = mRam.Read(addr);
  } else if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    data = mPpu->CpuRead(addr, bReadOnly);
  } else if (CONTROLLER_START <= addr && addr <= CONTROLLER_END) {
    data = mController->Read(addr);
  } else if (CPU_CARTRIDGE_START <= addr && addr <= CPU_CARTRIDGE_END) {
    data = mCartridge->CpuRead(addr);
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
