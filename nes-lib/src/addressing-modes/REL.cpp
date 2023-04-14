#include "addressing-modes/REL.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

REL::REL(Cpu* cpu) : IAddressingMode(cpu) {}

bool REL::operator()() {
  mCpu->addrRel = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  if (mCpu->addrRel & (1 << 7)) {
    mCpu->addrRel |= 0xFF00;
  }
  return false;
}
