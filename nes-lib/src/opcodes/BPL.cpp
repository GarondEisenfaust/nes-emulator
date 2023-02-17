#include "opcodes/BPL.h"
#include "Processor6502.h"

BPL::BPL(Processor6502* cpu) : IOpcode(cpu) {}

bool BPL::operator()() { return mCpu->BranchIf(mCpu->status.n == 0); }

const char* BPL::Name() {
  return "BPL";
}
