#include "opcodes/CMP.h"
#include "Processor6502.h"

CMP::CMP(Processor6502* cpu) : IOpcode(cpu) {}

bool CMP::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->a - mCpu->fetched;
  mCpu->status.c = mCpu->a >= mCpu->fetched;
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
  return true;
}

const char* CMP::Name() {
  return "CMP";
}
