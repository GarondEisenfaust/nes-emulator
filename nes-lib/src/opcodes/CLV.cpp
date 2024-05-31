#include "opcodes/CLV.h"
#include "Cpu.h"

CLV::CLV(Cpu* cpu) : IOpcode(cpu) {}

void CLV::operator()() { mCpu->status.v = false; }

const char* CLV::Name() {
  return "CLV";
}
