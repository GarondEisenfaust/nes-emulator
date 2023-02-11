#include "opcodes/DEX.h"
#include "Processor6502.h"

DEX::DEX(Processor6502* cpu) : IOpcode(cpu) {}

bool DEX::operator()() {
  mCpu->x--;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return 0;
}

const char* DEX::Name() {
  return "DEX";
}
