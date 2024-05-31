#include "opcodes/DEY.h"
#include "Cpu.h"

DEY::DEY(Cpu* cpu) : IOpcode(cpu) {}

void DEY::operator()() {
  mCpu->y--;
  mCpu->status.z = mCpu->y == 0;
  mCpu->status.n = mCpu->y & (1 << 7);
}

const char* DEY::Name() {
  return "DEY";
}
