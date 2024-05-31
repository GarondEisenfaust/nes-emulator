#include "opcodes/RTS.h"
#include "Cpu.h"

RTS::RTS(Cpu* cpu) : IOpcode(cpu) {}

void RTS::operator()() {
  mCpu->pc = mCpu->PopFromStack();
  mCpu->pc |= mCpu->PopFromStack() << 8;
  mCpu->pc++;
}

const char* RTS::Name() {
  return "RTS";
}
