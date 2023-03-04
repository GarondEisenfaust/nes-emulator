#include "opcodes/LDX.h"
#include "Cpu.h"

LDX::LDX(Cpu* cpu) : IOpcode(cpu) {}

bool LDX::operator()() {
  mCpu->Fetch();
  mCpu->x = mCpu->fetched;
  mCpu->status.z = mCpu->x == 0;
  mCpu->status.n = mCpu->x & (1 << 7);
  return true;
}

const char* LDX::Name() { return "LDX"; }
