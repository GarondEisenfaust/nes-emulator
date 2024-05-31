#include "opcodes/BMI.h"
#include "Cpu.h"

BMI::BMI(Cpu* cpu) : IOpcode(cpu) {}

void BMI::operator()() { mCpu->BranchIf(mCpu->status.n == true); }

const char* BMI::Name() {
  return "BMI";
}
