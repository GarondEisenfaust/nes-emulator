#include "opcodes/AND.h"
#include "Processor6502.h"

AND::AND(Processor6502* cpu) : IOpcode(cpu) {}

bool AND::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a & mCpu->fetched;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return 1;
}
