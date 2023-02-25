#include "opcodes/TAX.h"
#include "Cpu.h"

TAX::TAX(Cpu* cpu) : IOpcode(cpu) {}

bool TAX::operator()() {
  mCpu->x = mCpu->a;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return false;
}

const char* TAX::Name() { return "TAX"; }
