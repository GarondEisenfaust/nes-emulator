#include "opcodes/BVC.h"
#include "Cpu.h"

BVC::BVC(Cpu* cpu) : IOpcode(cpu) {}

bool BVC::operator()() { return mCpu->BranchIf(mCpu->status.v == 0); }
