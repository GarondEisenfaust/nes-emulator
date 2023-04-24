#include "opcodes/PHA.h"
#include "Cpu.h"

PHA::PHA(Cpu* cpu) : IOpcode(cpu) {}

void PHA::operator()() { mCpu->PushToStack(mCpu->a); }
