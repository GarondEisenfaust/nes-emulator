#include "opcodes/CLV.h"
#include "Processor6502.h"

CLV::CLV(Processor6502* cpu) : IOpcode(cpu) {}

bool CLV::operator()() {
  mCpu->status.v = false;
  return false;
}

const char* CLV::Name() {
  return "CLV";
}
