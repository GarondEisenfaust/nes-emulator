#include "IMM.h"
#include "Processor6502.h"

IMM::IMM(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IMM::operator()() {
  mCpu->addrAbs = mCpu->pc++;
  return 0;
}
