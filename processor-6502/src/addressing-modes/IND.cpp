#include "IND.h"
#include "Processor6502.h"

IND::IND(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IND::operator()() {
  uint16_t ptrLow = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t ptrHigh = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  uint16_t ptr = (ptrHigh << 8) | ptrLow;

  if (ptrLow == 0x00FF) {
    mCpu->addrAbs = (mCpu->Read(ptr & 0xFF00) << 8) | mCpu->Read(ptr + 0);
  } else {
    mCpu->addrAbs = (mCpu->Read(ptr + 1) << 8) | mCpu->Read(ptr + 0);
  }
  return 0;
}
