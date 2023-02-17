#include "opcodes/STX.h"
#include "Processor6502.h"

STX::STX(Processor6502* cpu) : IOpcode(cpu) {}

bool STX::operator()() {
  mCpu->Write(mCpu->addrAbs, mCpu->x);
  return false;
}

const char* STX::Name() {
  return "STX";
}
