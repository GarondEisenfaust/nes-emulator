#include "opcodes/BVS.h"
#include "Processor6502.h"

BVS::BVS(Processor6502* cpu) : IOpcode(cpu) {}

bool BVS::operator()() { return mCpu->BranchIf(mCpu->status.v == 1); }

const char* BVS::Name() {
  return "BVS";
}
