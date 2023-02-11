#include "ABS.h"
#include "Processor6502.h"

ABS::ABS(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ABS::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = (high << 8) | low;

  return 0;
}
