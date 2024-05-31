#include "opcodes/PHP.h"
#include "Cpu.h"

PHP::PHP(Cpu* cpu) : IOpcode(cpu) {}

void PHP::operator()() {
  auto tempStatus = mCpu->status;
  tempStatus.b = true;
  tempStatus.u = true;

  mCpu->PushToStack(tempStatus.reg);
  mCpu->status.b = false;
  mCpu->status.u = false;
}

const char* PHP::Name() {
  return "PHP";
}
