#include "addressing-modes/ZPY.h"
#include "Bus.h"
#include "Cpu.h"

ZPY::ZPY(Cpu* cpu) : IAddressingMode(cpu) {}

bool ZPY::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc) + mCpu->y;
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return false;
}
