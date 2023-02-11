#include "addressing-modes/IZX.h"
#include "Processor6502.h"

IZX::IZX(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IZX::operator()() {
  uint16_t t = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  uint16_t lo = mCpu->Read((t + mCpu->x) & 0x00FF);
  uint16_t hi = mCpu->Read((t + mCpu->x + 1) & 0x00FF);

  mCpu->addrAbs = (hi << 8) | lo;

  return 0;
}
