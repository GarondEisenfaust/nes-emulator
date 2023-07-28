#include "addressing-modes/IMP.h"
#include "Bus.h"
#include "Cpu.h"

IMP::IMP(Cpu* cpu) : IAddressingMode(cpu) {}

bool IMP::operator()() {
  mCpu->fetched = mCpu->a;
  return false;
}
