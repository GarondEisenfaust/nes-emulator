#include "opcodes/BVC.h"
#include "Processor6502.h"

BVC::BVC(Processor6502* cpu) : IOpcode(cpu) {}

bool BVC::operator()() { return mCpu->BranchIf(mCpu->status.v == 0); }

const char* BVC::Name() {
  return "BVC";
}
