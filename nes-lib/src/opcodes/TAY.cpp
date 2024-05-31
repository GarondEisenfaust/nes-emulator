#include "opcodes/TAY.h"
#include "Cpu.h"

TAY::TAY(Cpu* cpu) : IOpcode(cpu) {}

void TAY::operator()() {
  mCpu->y = mCpu->a;
  mCpu->status.z = mCpu->y == 0;
  mCpu->status.n = mCpu->y & (1 << 7);
}

const char* TAY::Name() {
  return "TAY";
}
