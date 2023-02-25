#include "opcodes/SEC.h"
#include "Cpu.h"

SEC::SEC(Cpu* cpu) : IOpcode(cpu) {}

bool SEC::operator()() {
  mCpu->status.c = true;
  return 0;
}

const char* SEC::Name() { return "SEC"; }
