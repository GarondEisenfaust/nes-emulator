#include "addressing-modes/ABX.h"
#include "Bus.h"
#include "Cpu.h"

ABX::ABX(Cpu* cpu) : IAddressingMode(cpu) {}

bool ABX::operator()() {
  uint16_t addr = mCpu->ReadTwoBytes(mCpu->pc);
  mCpu->pc += 2;
  mCpu->addrAbs = addr + mCpu->x;
  return (mCpu->addrAbs & 0xFF00) != (addr & 0xFF00);
}
