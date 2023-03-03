#include "opcodes/RTI.h"
#include "Cpu.h"

RTI::RTI(Cpu* cpu) : IOpcode(cpu) {}

bool RTI::operator()() {
  mCpu->status.reg = mCpu->PopFromStack();

  constexpr uint8_t allButB = ~(1 << 4);
  constexpr uint8_t allButU = ~(1 << 5);

  mCpu->status.reg &= allButB;
  mCpu->status.reg &= allButU;

  mCpu->pc = mCpu->PopFromStack();
  mCpu->pc |= mCpu->PopFromStack() << 8;
  return 0;
}

const char* RTI::Name() { return "RTI"; }
