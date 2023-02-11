#include "opcodes/PHA.h"
#include "Processor6502.h"

PHA::PHA(Processor6502* cpu) : IOpcode(cpu) {}

bool PHA::operator()() {
  mCpu->PushToStack(mCpu->a);
  return 0;
}
