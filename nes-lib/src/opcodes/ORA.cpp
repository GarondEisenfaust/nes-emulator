#include "opcodes/ORA.h"
#include "Cpu.h"

ORA::ORA(Cpu* cpu) : IOpcode(cpu) {}

bool ORA::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a | mCpu->fetched;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return true;
}

const char* ORA::Name() { return "ORA"; }
