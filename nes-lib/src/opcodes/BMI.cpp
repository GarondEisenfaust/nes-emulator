#include "opcodes/BMI.h"
#include "Cpu.h"

BMI::BMI(Cpu* cpu) : IOpcode(cpu) {}

bool BMI::operator()() { return mCpu->BranchIf(mCpu->status.n == 1); }

const char* BMI::Name() { return "BMI"; }
