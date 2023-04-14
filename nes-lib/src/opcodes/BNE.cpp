#include "opcodes/BNE.h"
#include "Cpu.h"

BNE::BNE(Cpu* cpu) : IOpcode(cpu) {}

bool BNE::operator()() { return mCpu->BranchIf(mCpu->status.z == 0); }
