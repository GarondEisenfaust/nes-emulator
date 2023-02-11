#include "opcodes/LDA.h"
#include "Processor6502.h"

LDA::LDA(Processor6502* cpu) : IOpcode(cpu) {}

bool LDA::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->fetched;
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return true;
}
