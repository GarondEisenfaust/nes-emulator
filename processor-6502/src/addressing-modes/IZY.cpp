#include "addressing-modes/IZY.h"
#include "Processor6502.h"

IZY::IZY(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IZY::operator()() {
  uint16_t t = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  uint16_t lo = mCpu->Read(t & 0x00FF);
  uint16_t hi = mCpu->Read((t + 1) & 0x00FF);

  mCpu->addrAbs = ((hi << 8) | lo) + mCpu->y;

  if ((mCpu->addrAbs & 0xFF00) != (hi << 8)) {
    return 1;
  } else {
    return 0;
  }
}
