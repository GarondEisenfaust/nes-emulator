#include "opcodes/LDX.h"
#include "Processor6502.h"

LDX::LDX(Processor6502* cpu) : IOpcode(cpu) {}

bool LDX::operator()() {
  mCpu->Fetch();
  mCpu->x = mCpu->fetched;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return true;
}

const char* LDX::Name() {
  return "LDX";
}
