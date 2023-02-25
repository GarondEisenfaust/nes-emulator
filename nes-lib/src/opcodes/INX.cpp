#include "opcodes/INX.h"
#include "Processor6502.h"

INX::INX(Processor6502* cpu) : IOpcode(cpu) {}

bool INX::operator()() {
  mCpu->x++;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return false;
}

const char* INX::Name() {
  return "INX";
}