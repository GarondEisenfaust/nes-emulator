#include "opcodes/STA.h"
#include "Cpu.h"

STA::STA(Cpu* cpu) : IOpcode(cpu) {}

bool STA::operator()() {
  mCpu->Write(mCpu->addrAbs, mCpu->a);
  return false;
}
