#include "opcodes/SEI.h"
#include "Cpu.h"

SEI::SEI(Cpu* cpu) : IOpcode(cpu) {}

void SEI::operator()() { mCpu->status.i = true; }
