#include "opcodes/TAY.h"
#include "Processor6502.h"

TAY::TAY(Processor6502* cpu) : IOpcode(cpu) {}

bool TAY::operator()() {
  mCpu->y = mCpu->a;
  mCpu->status.z = mCpu->y == 0x00;
  mCpu->status.n = mCpu->y & 0x80;
  return false;
}
