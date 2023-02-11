#include "addressing-modes/REL.h"
#include "Processor6502.h"

REL::REL(Processor6502* cpu) : IAddressingMode(cpu) {}

bool REL::operator()() {
  mCpu->addrRel = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  if (mCpu->addrRel & 0x80) {
    mCpu->addrRel |= 0xFF00;
  }
  return 0;
}
