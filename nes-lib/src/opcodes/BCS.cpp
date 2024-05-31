#include "opcodes/BCS.h"
#include "Cpu.h"

BCS::BCS(Cpu* cpu) : IOpcode(cpu) {}

void BCS::operator()() { mCpu->BranchIf(mCpu->status.c == true); }

const char* BCS::Name() {
  return "BCS";
}
