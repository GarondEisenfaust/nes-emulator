#include "opcodes/TXS.h"
#include "Cpu.h"

TXS::TXS(Cpu* cpu) : IOpcode(cpu) {}

bool TXS::operator()() {
  mCpu->stackPointer = mCpu->x;
  return 0;
}
