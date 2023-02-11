#include "opcodes/LDY.h"
#include "Processor6502.h"

LDY::LDY(Processor6502* cpu) : IOpcode(cpu) {}

bool LDY::operator()() {
  mCpu->Fetch();
  mCpu->y = mCpu->fetched;
  mCpu->status.z = mCpu->y == 0x00;
  mCpu->status.n = mCpu->y & 0x80;
  return true;
}

const char* LDY::Name() {
  return "LDY";
}
