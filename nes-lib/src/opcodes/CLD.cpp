#include "opcodes/CLD.h"
#include "Cpu.h"

CLD::CLD(Cpu* cpu) : IOpcode(cpu) {}

void CLD::operator()() { mCpu->status.d = false; }

const char* CLD::Name() {
  return "CLD";
}
