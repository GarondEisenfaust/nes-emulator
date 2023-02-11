#include "opcodes/INY.h"
#include "Processor6502.h"

INY::INY(Processor6502* cpu) : IOpcode(cpu) {}

bool INY::operator()() {
  mCpu->y++;
  mCpu->status.z = mCpu->y == 0x00;
  mCpu->status.n = mCpu->y & 0x80;
  return false;
}
