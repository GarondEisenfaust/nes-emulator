#include "opcodes/RTI.h"
#include "Processor6502.h"

RTI::RTI(Processor6502* cpu) : IOpcode(cpu) {}

bool RTI::operator()() {
  mCpu->status.reg = mCpu->PopFromStack();
  mCpu->status.reg &= ~B;
  mCpu->status.reg &= ~U;

  mCpu->pc = mCpu->PopFromStack();
  mCpu->pc |= mCpu->PopFromStack() << 8;
  return 0;
}
