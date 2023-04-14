#include "addressing-modes/IZY.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

IZY::IZY(Cpu* cpu) : IAddressingMode(cpu) {}

bool IZY::operator()() {
  uint16_t t = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  uint16_t lo = mCpu->Read(t & 0x00FF);
  uint16_t hi = mCpu->Read((t + 1) & 0x00FF);

  mCpu->addrAbs = ((hi << 8) | lo) + mCpu->y;
  return (mCpu->addrAbs & 0xFF00) != (hi << 8);
}
