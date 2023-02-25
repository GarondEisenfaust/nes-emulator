#include "opcodes/PHP.h"
#include "Cpu.h"

PHP::PHP(Cpu* cpu) : IOpcode(cpu) {}

bool PHP::operator()() {
  mCpu->PushToStack(mCpu->status.reg | B | U);
  mCpu->status.b = false;
  mCpu->status.u = false;
  return 0;
}

const char* PHP::Name() { return "PHP"; }
