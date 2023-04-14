#include "opcodes/BCC.h"
#include "Cpu.h"

BCC::BCC(Cpu* cpu) : IOpcode(cpu) {}

bool BCC::operator()() { return mCpu->BranchIf(mCpu->status.c == 0); }
