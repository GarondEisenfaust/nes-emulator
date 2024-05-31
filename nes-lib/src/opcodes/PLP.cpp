#include "opcodes/PLP.h"
#include "Cpu.h"

PLP::PLP(Cpu* cpu) : IOpcode(cpu) {}

void PLP::operator()() {
  mCpu->status.reg = mCpu->PopFromStack();
  mCpu->status.u = true;
}

const char* PLP::Name() {
  return "PLP";
}
