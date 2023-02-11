#include "opcodes/TSX.h"
#include "Processor6502.h"

TSX::TSX(Processor6502* cpu) : IOpcode(cpu) {}

bool TSX::operator()() {
  mCpu->x = mCpu->stackPointer;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return false;
}
