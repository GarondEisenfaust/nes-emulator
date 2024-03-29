#include "opcodes/LDY.h"
#include "Cpu.h"

LDY::LDY(Cpu* cpu) : IOpcode(cpu) {}

void LDY::operator()() {
  mCpu->Fetch();
  mCpu->y = mCpu->fetched;
  mCpu->status.z = mCpu->y == 0;
  mCpu->status.n = mCpu->y & (1 << 7);
}
