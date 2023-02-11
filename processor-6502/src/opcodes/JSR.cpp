#include "opcodes/JSR.h"
#include "Processor6502.h"

JSR::JSR(Processor6502* cpu) : IOpcode(cpu) {}

bool JSR::operator()() {
  mCpu->pc--;
  mCpu->PushToStack((mCpu->pc >> 8) & 0x00FF);
  mCpu->PushToStack(mCpu->pc & 0x00FF);

  mCpu->pc = mCpu->addrAbs;
  return false;
}
