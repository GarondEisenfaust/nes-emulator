#include "opcodes/PLP.h"
#include "Processor6502.h"

PLP::PLP(Processor6502* cpu) : IOpcode(cpu) {}

bool PLP::operator()() {
  mCpu->status.reg = mCpu->PopFromStack();
  mCpu->status.u = true;
  return 0;
}
