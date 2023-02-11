#include "addressing-modes/ABY.h"
#include "Processor6502.h"

ABY::ABY(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ABY::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = ((high << 8) | low) + mCpu->y;

  if ((mCpu->addrAbs & 0xFF00) != (high << 8)) {
    return 1;
  } else {
    return 0;
  }
}
