#include "opcodes/CLD.h"
#include "Processor6502.h"

CLD::CLD(Processor6502* cpu) : IOpcode(cpu) {}

bool CLD::operator()() {
  mCpu->status.d = false;
  return false;
}
