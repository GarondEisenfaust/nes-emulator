#include "opcodes/CLC.h"
#include "Cpu.h"

CLC::CLC(Cpu* cpu) : IOpcode(cpu) {}

bool CLC::operator()() {
  mCpu->status.c = false;
  return false;
}

const char* CLC::Name() { return "CLC"; }
