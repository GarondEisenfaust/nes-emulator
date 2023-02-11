#include "addressing-modes/ZPY.h"
#include "Processor6502.h"

ZPY::ZPY(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ZPY::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc) + mCpu->y;
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return 0;
}
