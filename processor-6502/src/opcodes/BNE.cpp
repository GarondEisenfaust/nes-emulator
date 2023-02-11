#include "opcodes/BNE.h"
#include "Processor6502.h"

BNE::BNE(Processor6502* cpu) : IOpcode(cpu) {}

bool BNE::operator()() { return mCpu->BranchIf(mCpu->status.z == 0); }
