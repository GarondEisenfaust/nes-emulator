#include "opcodes/BMI.h"
#include "Processor6502.h"

BMI::BMI(Processor6502* cpu) : IOpcode(cpu) {}

bool BMI::operator()() { return mCpu->BranchIf(mCpu->status.n == 1); }
