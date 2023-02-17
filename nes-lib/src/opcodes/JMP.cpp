#include "opcodes/JMP.h"
#include "Processor6502.h"

JMP::JMP(Processor6502* cpu) : IOpcode(cpu) {}

bool JMP::operator()() {
  mCpu->pc = mCpu->addrAbs;
  return 0;
}

const char* JMP::Name() {
  return "JMP";
}
