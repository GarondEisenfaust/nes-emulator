#include "opcodes/TYA.h"
#include "Processor6502.h"

TYA::TYA(Processor6502* cpu) : IOpcode(cpu) {}

bool TYA::operator()() {
  mCpu->a = mCpu->y;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return false;
}

const char* TYA::Name() {
  return "TYA";
}
