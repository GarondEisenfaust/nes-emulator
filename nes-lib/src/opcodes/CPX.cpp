#include "opcodes/CPX.h"
#include "Cpu.h"

CPX::CPX(Cpu* cpu) : IOpcode(cpu) {}

bool CPX::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->x - mCpu->fetched;
  mCpu->status.c = mCpu->x >= mCpu->fetched;
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
  return true;
}

const char* CPX::Name() { return "CPX"; }
