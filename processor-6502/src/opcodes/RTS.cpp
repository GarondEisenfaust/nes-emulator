#include "opcodes/RTS.h"
#include "Processor6502.h"

RTS::RTS(Processor6502* cpu) : IOpcode(cpu) {}

bool RTS::operator()() {
  mCpu->pc = mCpu->PopFromStack();
  mCpu->pc |= mCpu->PopFromStack() << 8;

  mCpu->pc++;
  return 0;
}
