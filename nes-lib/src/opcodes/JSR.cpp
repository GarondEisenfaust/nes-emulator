#include "opcodes/JSR.h"
#include "Cpu.h"

JSR::JSR(Cpu* cpu) : IOpcode(cpu) {}

void JSR::operator()() {
  mCpu->pc--;
  mCpu->PushToStack((mCpu->pc >> 8) & 0x00FF);
  mCpu->PushToStack(mCpu->pc & 0x00FF);
  mCpu->pc = mCpu->addrAbs;
}
