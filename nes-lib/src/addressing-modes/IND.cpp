#include "addressing-modes/IND.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

IND::IND(Cpu* cpu) : IAddressingMode(cpu) {}

bool IND::operator()() {
  uint16_t ptr = mCpu->ReadTwoBytes(mCpu->pc);
  mCpu->pc += 2;

  if ((ptr & 0x00FF) == 0x00FF) {
    mCpu->addrAbs = (mCpu->Read(ptr & 0xFF00) << 8) | mCpu->Read(ptr + 0);
  } else {
    mCpu->addrAbs = (mCpu->Read(ptr + 1) << 8) | mCpu->Read(ptr + 0);
  }
  return false;
}
