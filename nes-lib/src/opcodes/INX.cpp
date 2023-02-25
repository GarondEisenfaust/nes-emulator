#include "opcodes/INX.h"
#include "Cpu.h"

INX::INX(Cpu* cpu) : IOpcode(cpu) {}

bool INX::operator()() {
  mCpu->x++;
  mCpu->status.z = mCpu->x == 0x00;
  mCpu->status.n = mCpu->x & 0x80;
  return false;
}

const char* INX::Name() { return "INX"; }
