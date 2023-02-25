#include "opcodes/RTI.h"
#include "Cpu.h"

RTI::RTI(Cpu* cpu) : IOpcode(cpu) {}

bool RTI::operator()() {
  mCpu->status.reg = mCpu->PopFromStack();
  mCpu->status.reg &= ~B;
  mCpu->status.reg &= ~U;

  mCpu->pc = mCpu->PopFromStack();
  mCpu->pc |= mCpu->PopFromStack() << 8;
  return 0;
}

const char* RTI::Name() { return "RTI"; }
