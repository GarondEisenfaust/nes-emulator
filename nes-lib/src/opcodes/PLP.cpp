#include "opcodes/PLP.h"
#include "Cpu.h"

PLP::PLP(Cpu* cpu) : IOpcode(cpu) {}

bool PLP::operator()() {
  mCpu->status.reg = mCpu->PopFromStack();
  mCpu->status.u = true;
  return 0;
}

const char* PLP::Name() { return "PLP"; }
