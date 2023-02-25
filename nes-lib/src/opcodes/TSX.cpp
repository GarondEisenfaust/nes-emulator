#include "opcodes/TSX.h"
#include "Cpu.h"

TSX::TSX(Cpu* cpu) : IOpcode(cpu) {}

bool TSX::operator()() {
  mCpu->x = mCpu->stackPointer;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return false;
}

const char* TSX::Name() { return "TSX"; }
