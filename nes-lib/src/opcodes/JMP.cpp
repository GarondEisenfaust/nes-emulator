#include "opcodes/JMP.h"
#include "Cpu.h"

JMP::JMP(Cpu* cpu) : IOpcode(cpu) {}

bool JMP::operator()() {
  mCpu->pc = mCpu->addrAbs;
  return 0;
}
