#include "opcodes/STA.h"
#include "Processor6502.h"

STA::STA(Processor6502* cpu) : IOpcode(cpu) {}

bool STA::operator()() {
  mCpu->Write(mCpu->addrAbs, mCpu->a);
  return false;
}
