#include "opcodes/BEQ.h"
#include "Processor6502.h"

BEQ::BEQ(Processor6502* cpu) : IOpcode(cpu) {}

bool BEQ::operator()() { return mCpu->BranchIf(mCpu->status.z == 1); }

const char* BEQ::Name() {
  return "BEQ";
}
