#include "opcodes/BNE.h"
#include "Cpu.h"

BNE::BNE(Cpu* cpu) : IOpcode(cpu) {}

void BNE::operator()() { mCpu->BranchIf(mCpu->status.z == false); }
