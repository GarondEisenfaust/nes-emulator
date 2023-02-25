#include "opcodes/CLD.h"
#include "Cpu.h"

CLD::CLD(Cpu* cpu) : IOpcode(cpu) {}

bool CLD::operator()() {
  mCpu->status.d = false;
  return false;
}

const char* CLD::Name() { return "CLD"; }
