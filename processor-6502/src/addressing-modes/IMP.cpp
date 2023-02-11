#include "addressing-modes/IMP.h"
#include "Processor6502.h"

IMP::IMP(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IMP::operator()() {
  mCpu->fetched = mCpu->a;
  return 0;
}
