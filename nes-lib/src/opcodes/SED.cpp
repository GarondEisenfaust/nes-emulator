#include "opcodes/SED.h"
#include "Cpu.h"

SED::SED(Cpu* cpu) : IOpcode(cpu) {}

bool SED::operator()() {
  mCpu->status.d = true;
  return 0;
}
