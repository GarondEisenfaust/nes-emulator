#include "opcodes/BEQ.h"
#include "Cpu.h"

BEQ::BEQ(Cpu* cpu) : IOpcode(cpu) {}

bool BEQ::operator()() { return mCpu->BranchIf(mCpu->status.z == 1); }
