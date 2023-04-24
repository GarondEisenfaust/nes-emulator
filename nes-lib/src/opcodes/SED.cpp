#include "opcodes/SED.h"
#include "Cpu.h"

SED::SED(Cpu* cpu) : IOpcode(cpu) {}

void SED::operator()() { mCpu->status.d = true; }
