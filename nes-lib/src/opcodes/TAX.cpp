#include "opcodes/TAX.h"
#include "Processor6502.h"

TAX::TAX(Processor6502* cpu) : IOpcode(cpu) {}

bool TAX::operator()() {
  mCpu->x = mCpu->a;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return false;
}

const char* TAX::Name() {
  return "TAX";
}
