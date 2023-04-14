#include "opcodes/LDA.h"
#include "Cpu.h"

LDA::LDA(Cpu* cpu) : IOpcode(cpu) {}

bool LDA::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->fetched;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
  return true;
}
