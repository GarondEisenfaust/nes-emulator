#include "opcodes/TXS.h"
#include "Processor6502.h"

TXS::TXS(Processor6502* cpu) : IOpcode(cpu) {}

bool TXS::operator()() {
  mCpu->stackPointer = mCpu->x;
  return 0;
}
