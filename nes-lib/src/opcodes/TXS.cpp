#include "opcodes/TXS.h"
#include "Cpu.h"

TXS::TXS(Cpu* cpu) : IOpcode(cpu) {}

void TXS::operator()() { mCpu->stackPointer = mCpu->x; }

const char* TXS::Name() {
  return "TXS";
}
