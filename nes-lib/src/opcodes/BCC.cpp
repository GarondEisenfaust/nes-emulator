#include "opcodes/BCC.h"
#include "Cpu.h"

BCC::BCC(Cpu* cpu) : IOpcode(cpu) {}

void BCC::operator()() { mCpu->BranchIf(mCpu->status.c == 0); }

const char* BCC::Name() {
  return "BCC";
}
