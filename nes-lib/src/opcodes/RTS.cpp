#include "opcodes/RTS.h"
#include "Cpu.h"

RTS::RTS(Cpu* cpu) : IOpcode(cpu) {}

bool RTS::operator()() {
  mCpu->pc = mCpu->PopFromStack();
  mCpu->pc |= mCpu->PopFromStack() << 8;

  mCpu->pc++;
  return 0;
}

const char* RTS::Name() { return "RTS"; }
