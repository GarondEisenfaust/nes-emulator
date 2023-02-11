#include "ZP0.h"
#include "Processor6502.h"

ZP0::ZP0(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ZP0::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return 0;
}
