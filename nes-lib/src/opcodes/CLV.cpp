#include "opcodes/CLV.h"
#include "Cpu.h"

CLV::CLV(Cpu* cpu) : IOpcode(cpu) {}

bool CLV::operator()() {
  mCpu->status.v = false;
  return false;
}

const char* CLV::Name() { return "CLV"; }
