#include "opcodes/SED.h"
#include "Processor6502.h"

SED::SED(Processor6502* cpu) : IOpcode(cpu) {}

bool SED::operator()() {
  mCpu->status.d = true;
  return 0;
}
