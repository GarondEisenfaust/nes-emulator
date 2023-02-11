#include "opcodes/STY.h"
#include "Processor6502.h"

STY::STY(Processor6502* cpu) : IOpcode(cpu) {}

bool STY::operator()() {
  mCpu->Write(mCpu->addrAbs, mCpu->y);
  return false;
}
