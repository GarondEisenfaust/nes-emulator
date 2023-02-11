#include "opcodes/SEC.h"
#include "Processor6502.h"

SEC::SEC(Processor6502* cpu) : IOpcode(cpu) {}

bool SEC::operator()() {
  mCpu->status.c = true;
  return 0;
}

const char* SEC::Name() {
  return "SEC";
}
