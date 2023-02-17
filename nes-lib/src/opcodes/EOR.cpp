#include "opcodes/EOR.h"
#include "Processor6502.h"

EOR::EOR(Processor6502* cpu) : IOpcode(cpu) {}

bool EOR::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a ^ mCpu->fetched;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return true;
}

const char* EOR::Name() {
  return "EOR";
}
