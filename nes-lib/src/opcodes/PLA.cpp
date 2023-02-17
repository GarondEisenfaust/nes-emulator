#include "opcodes/PLA.h"
#include "Processor6502.h"

PLA::PLA(Processor6502* cpu) : IOpcode(cpu) {}

bool PLA::operator()() {
  mCpu->a = mCpu->PopFromStack();
  mCpu->status.z = mCpu->a == 0x00;
  mCpu->status.n = mCpu->a & 0x80;
  return 0;
}

const char* PLA::Name() {
  return "PLA";
}
