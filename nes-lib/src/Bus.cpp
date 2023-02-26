#include "Bus.h"
#include "Definitions.h"
#include "IRenderer.h"

Bus::Bus(RAM& ram) : mRam(ram), mSystemClockCounter(0) {}

Bus::~Bus() {}

void Bus::CpuWrite(uint16_t addr, uint8_t data) {
  if (mCartridge->CpuWrite(addr, data)) {
  } else if (RAM_START <= addr && addr <= RAM_END) {
    mRam[addr & RAM_SIZE] = data;
  } else if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    mPpu->CpuWrite(addr & PPU_RAM_SIZE, data);
  } else if (0x4014 == addr) {
    mDma.page = data;
    mDma.addr = 0x00;
    mDma.transfer = true;
  } else if (0x4016 <= addr && addr <= 0x4017) {
    mController->Write(addr);
  }
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly) {
  uint8_t data = 0x00;

  if (mCartridge->CpuRead(addr, data)) {
  } else if (RAM_START <= addr && addr <= RAM_END) {
    data = mRam[addr & RAM_SIZE];
  } else if (PPU_RAM_START <= addr && addr <= PPU_RAM_END) {
    data = mPpu->CpuRead(addr & PPU_RAM_SIZE, bReadOnly);
  } else if (0x4016 <= addr && addr <= 0x4017) {
    data = mController->Read(addr);
  }
  return data;
}

void Bus::ConnectController(Controller* controller) { mController = controller; }

void Bus::InsertCartridge(Cartridge* cartridge) {
  mCartridge = cartridge;
  mPpu->InsertCartridge(cartridge);
}

void Bus::Reset() {
  mCartridge->Reset();
  mCpu->Reset();
  mPpu->Reset();
  mSystemClockCounter = 0;
  mDma.page = 0x00;
  mDma.addr = 0x00;
  mDma.data = 0x00;
  mDma.dummy = true;
  mDma.transfer = false;
}

void Bus::Clock() {
  mPpu->Clock();
  if (mSystemClockCounter % 3 == 0) {
    if (mDma.transfer) {
      if (mDma.dummy) {
        if (mSystemClockCounter % 2 == 1) {
          mDma.dummy = false;
        }
      } else {
        if (mSystemClockCounter % 2 == 0) {
          mDma.data = CpuRead(mDma.page << 8 | mDma.addr);
        }
        if (mSystemClockCounter % 2 == 1) {
          mPpu->mOamPtr[mDma.addr] = mDma.data;
          mDma.addr++;
          if (mDma.addr == 0x00) {
            mDma.transfer = false;
            mDma.dummy = true;
          }
        }
      }
    } else {
      mCpu->Clock();
    }
  }

  if (mPpu->nmi) {
    mPpu->nmi = false;
    mCpu->NonMaskableInterrupt();
  }

  mSystemClockCounter++;
}
