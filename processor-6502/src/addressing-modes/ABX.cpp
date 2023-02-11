#include "addressing-modes/ABX.h"
#include "Processor6502.h"

ABX::ABX(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ABX::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = ((high << 8) | low) + mCpu->x;

  if ((mCpu->addrAbs & 0xFF00) != (high << 8)) {
    return 1;
  } else {
    return 0;
  }
}
