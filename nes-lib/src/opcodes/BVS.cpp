#include "opcodes/BVS.h"
#include "Cpu.h"

BVS::BVS(Cpu* cpu) : IOpcode(cpu) {}

void BVS::operator()() { mCpu->BranchIf(mCpu->status.v == true); }

const char* BVS::Name() {
  return "BVS";
}
