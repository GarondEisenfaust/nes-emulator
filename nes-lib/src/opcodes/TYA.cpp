#include "opcodes/TYA.h"
#include "Cpu.h"

TYA::TYA(Cpu* cpu) : IOpcode(cpu) {}

bool TYA::operator()() {
  mCpu->a = mCpu->y;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
  return false;
}
