#include "opcodes/BEQ.h"
#include "Cpu.h"

BEQ::BEQ(Cpu* cpu) : IOpcode(cpu) {}

void BEQ::operator()() { mCpu->BranchIf(mCpu->status.z == true); }

const char* BEQ::Name() {
  return "BEQ";
}
