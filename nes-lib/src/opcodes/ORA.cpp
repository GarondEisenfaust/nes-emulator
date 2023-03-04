#include "opcodes/ORA.h"
#include "Cpu.h"

ORA::ORA(Cpu* cpu) : IOpcode(cpu) {}

bool ORA::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a | mCpu->fetched;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
  return true;
}

const char* ORA::Name() { return "ORA"; }
