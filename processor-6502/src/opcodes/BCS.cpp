#include "opcodes/BCS.h"
#include "Processor6502.h"

BCS::BCS(Processor6502* cpu) : IOpcode(cpu) {}

bool BCS::operator()() { return mCpu->BranchIf(mCpu->status.c == 1); }

const char* BCS::Name() {
  return "BCS";
}
