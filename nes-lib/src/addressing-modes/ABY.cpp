#include "addressing-modes/ABY.h"
#include "Bus.h"
#include "Cpu.h"

ABY::ABY(Cpu* cpu) : IAddressingMode(cpu) {}

bool ABY::operator()() {
  uint16_t addr = mCpu->ReadTwoBytes(mCpu->pc);
  mCpu->pc += 2;
  mCpu->addrAbs = addr + mCpu->y;
  return (mCpu->addrAbs & 0xFF00) != (addr & 0xFF00);
}
