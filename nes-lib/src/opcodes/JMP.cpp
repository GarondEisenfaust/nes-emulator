#include "opcodes/JMP.h"
#include "Cpu.h"

JMP::JMP(Cpu* cpu) : IOpcode(cpu) {}

void JMP::operator()() { mCpu->pc = mCpu->addrAbs; }
