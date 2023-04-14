#include "opcodes/PHA.h"
#include "Cpu.h"

PHA::PHA(Cpu* cpu) : IOpcode(cpu) {}

bool PHA::operator()() {
  mCpu->PushToStack(mCpu->a);
  return 0;
}
