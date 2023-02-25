#include "opcodes/DEY.h"
#include "Cpu.h"

DEY::DEY(Cpu* cpu) : IOpcode(cpu) {}

bool DEY::operator()() {
  mCpu->y--;
  mCpu->status.z = mCpu->y == 0x00;
  mCpu->status.n = mCpu->y & 0x80;
  return 0;
}

const char* DEY::Name() { return "DEY"; }
