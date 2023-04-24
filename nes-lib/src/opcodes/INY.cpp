#include "opcodes/INY.h"
#include "Cpu.h"

INY::INY(Cpu* cpu) : IOpcode(cpu) {}

void INY::operator()() {
  mCpu->y++;
  mCpu->status.z = mCpu->y == 0;
  mCpu->status.n = mCpu->y & (1 << 7);
}
