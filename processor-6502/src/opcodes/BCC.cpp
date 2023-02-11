#include "opcodes/BCC.h"
#include "Processor6502.h"

BCC::BCC(Processor6502* cpu) : IOpcode(cpu) {}

bool BCC::operator()() { return mCpu->BranchIf(mCpu->status.c == 0); }
