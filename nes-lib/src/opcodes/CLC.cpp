#include "opcodes/CLC.h"
#include "Processor6502.h"

CLC::CLC(Processor6502* cpu) : IOpcode(cpu) {}

bool CLC::operator()() {
  mCpu->status.c = false;
  return false;
}

const char* CLC::Name() {
  return "CLC";
}
