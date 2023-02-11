#include "addressing-modes/ZPX.h"
#include "Processor6502.h"

ZPX::ZPX(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ZPX::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc) + mCpu->x;
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return 0;
}
