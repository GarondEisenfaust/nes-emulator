#include "opcodes/DEX.h"
#include "Cpu.h"

DEX::DEX(Cpu* cpu) : IOpcode(cpu) {}

void DEX::operator()() {
  mCpu->x--;
  mCpu->status.z = mCpu->x == 0;
  mCpu->status.n = mCpu->x & (1 << 7);
}

const char* DEX::Name() {
  return "DEX";
}
