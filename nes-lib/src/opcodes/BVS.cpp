#include "opcodes/BVS.h"
#include "Cpu.h"

BVS::BVS(Cpu* cpu) : IOpcode(cpu) {}

bool BVS::operator()() { return mCpu->BranchIf(mCpu->status.v == 1); }

const char* BVS::Name() { return "BVS"; }
