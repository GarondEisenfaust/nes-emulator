#include "addressing-modes/ZPX.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

ZPX::ZPX(Cpu* cpu) : IAddressingMode(cpu) {}

bool ZPX::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc) + mCpu->x;
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return false;
}
