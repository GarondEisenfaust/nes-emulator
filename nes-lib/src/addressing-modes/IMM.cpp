#include "addressing-modes/IMM.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

IMM::IMM(Cpu* cpu) : IAddressingMode(cpu) {}

bool IMM::operator()() {
  mCpu->addrAbs = mCpu->pc++;
  return false;
}
