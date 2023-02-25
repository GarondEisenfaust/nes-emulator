#include "opcodes/BPL.h"
#include "Cpu.h"

BPL::BPL(Cpu* cpu) : IOpcode(cpu) {}

bool BPL::operator()() { return mCpu->BranchIf(mCpu->status.n == 0); }

const char* BPL::Name() { return "BPL"; }
