#include "opcodes/BVC.h"
#include "Cpu.h"

BVC::BVC(Cpu* cpu) : IOpcode(cpu) {}

void BVC::operator()() { mCpu->BranchIf(mCpu->status.v == 0); }
