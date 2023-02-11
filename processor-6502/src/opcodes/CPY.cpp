#include "opcodes/CPY.h"
#include "Processor6502.h"

CPY::CPY(Processor6502* cpu) : IOpcode(cpu) {}

bool CPY::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->y - mCpu->fetched;
  mCpu->status.c = mCpu->y >= mCpu->fetched;
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
  return true;
}

const char* CPY::Name() {
  return "CPY";
}
