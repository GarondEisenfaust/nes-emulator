#include "opcodes/TAY.h"
#include "Cpu.h"

TAY::TAY(Cpu* cpu) : IOpcode(cpu) {}

bool TAY::operator()() {
  mCpu->y = mCpu->a;
  mCpu->status.z = mCpu->y == 0x00;
  mCpu->status.n = mCpu->y & 0x80;
  return false;
}

const char* TAY::Name() { return "TAY"; }
