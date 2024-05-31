#include "opcodes/TAX.h"
#include "Cpu.h"

TAX::TAX(Cpu* cpu) : IOpcode(cpu) {}

void TAX::operator()() {
  mCpu->x = mCpu->a;
  mCpu->status.z = mCpu->x == 0;
  mCpu->status.n = mCpu->x & (1 << 7);
}

const char* TAX::Name() {
  return "TAX";
}
