#include "opcodes/CLC.h"
#include "Cpu.h"

CLC::CLC(Cpu* cpu) : IOpcode(cpu) {}

void CLC::operator()() { mCpu->status.c = false; }

const char* CLC::Name() {
  return "CLC";
}
