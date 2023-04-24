#include "opcodes/INX.h"
#include "Cpu.h"

INX::INX(Cpu* cpu) : IOpcode(cpu) {}

void INX::operator()() {
  mCpu->x++;
  mCpu->status.z = mCpu->x == 0;
  mCpu->status.n = mCpu->x & (1 << 7);
}
