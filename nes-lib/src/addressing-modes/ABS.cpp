#include "addressing-modes/ABS.h"
#include "Bus.h"
#include "Cpu.h"

ABS::ABS(Cpu* cpu) : IAddressingMode(cpu) {}

bool ABS::operator()() {
  mCpu->addrAbs = mCpu->ReadTwoBytes(mCpu->pc);
  mCpu->pc += 2;
  return false;
}
