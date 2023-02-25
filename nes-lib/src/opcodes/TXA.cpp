#include "opcodes/TXA.h"
#include "Cpu.h"

TXA::TXA(Cpu* cpu) : IOpcode(cpu) {}

bool TXA::operator()() {
  mCpu->a = mCpu->x;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return 0;
}

const char* TXA::Name() { return "TXA"; }
