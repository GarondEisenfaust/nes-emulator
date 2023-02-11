#include "opcodes/SEI.h"
#include "Processor6502.h"

SEI::SEI(Processor6502* cpu) : IOpcode(cpu) {}

bool SEI::operator()() {
  mCpu->status.i = true;
  return 0;
}
