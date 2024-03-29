#include "opcodes/TXA.h"
#include "Cpu.h"

TXA::TXA(Cpu* cpu) : IOpcode(cpu) {}

void TXA::operator()() {
  mCpu->a = mCpu->x;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
}
