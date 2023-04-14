#include "opcodes/BCS.h"
#include "Cpu.h"

BCS::BCS(Cpu* cpu) : IOpcode(cpu) {}

bool BCS::operator()() { return mCpu->BranchIf(mCpu->status.c == 1); }
