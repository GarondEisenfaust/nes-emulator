#include "opcodes/BRK.h"
#include "Processor6502.h"

BRK::BRK(Processor6502* cpu) : IOpcode(cpu) {}

bool BRK::operator()() {
  mCpu->pc++;

  mCpu->status.i = true;
  mCpu->PushToStack((mCpu->pc >> 8) & 0x00FF);
  mCpu->PushToStack(mCpu->pc & 0x00FF);

  mCpu->status.b = true;
  mCpu->PushToStack(mCpu->status.reg);
  mCpu->status.b = false;

  mCpu->pc = mCpu->Read(0xFFFE) | (mCpu->Read(0xFFFF) << 8);
  return 0;
}
