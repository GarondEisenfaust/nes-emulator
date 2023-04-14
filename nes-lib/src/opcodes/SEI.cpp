#include "opcodes/SEI.h"
#include "Cpu.h"

SEI::SEI(Cpu* cpu) : IOpcode(cpu) {}

bool SEI::operator()() {
  mCpu->status.i = true;
  return 0;
}
