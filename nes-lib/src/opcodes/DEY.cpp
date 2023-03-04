#include "opcodes/DEY.h"
#include "Cpu.h"

DEY::DEY(Cpu* cpu) : IOpcode(cpu) {}

bool DEY::operator()() {
  mCpu->y--;
  mCpu->status.z = mCpu->y == 0;
  mCpu->status.n = mCpu->y & (1 << 7);
  return 0;
}

const char* DEY::Name() { return "DEY"; }
