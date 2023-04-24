#include "opcodes/TSX.h"
#include "Cpu.h"

TSX::TSX(Cpu* cpu) : IOpcode(cpu) {}

void TSX::operator()() {
  mCpu->x = mCpu->stackPointer;
  mCpu->status.z = mCpu->x == 0;
  mCpu->status.n = mCpu->x & (1 << 7);
}
