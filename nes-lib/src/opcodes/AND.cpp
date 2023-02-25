#include "opcodes/AND.h"
#include "Cpu.h"

AND::AND(Cpu* cpu) : IOpcode(cpu) {}

bool AND::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a & mCpu->fetched;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return 1;
}

const char* AND::Name() { return "AND"; }
