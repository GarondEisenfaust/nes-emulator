#include "opcodes/BPL.h"
#include "Cpu.h"

BPL::BPL(Cpu* cpu) : IOpcode(cpu) {}

void BPL::operator()() { mCpu->BranchIf(mCpu->status.n == false); }
