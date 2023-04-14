#include "opcodes/AND.h"
#include "Cpu.h"

AND::AND(Cpu* cpu) : IOpcode(cpu) {}

bool AND::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a & mCpu->fetched;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
  return 1;
}
