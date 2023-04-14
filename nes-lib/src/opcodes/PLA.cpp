#include "opcodes/PLA.h"
#include "Cpu.h"

PLA::PLA(Cpu* cpu) : IOpcode(cpu) {}

bool PLA::operator()() {
  mCpu->a = mCpu->PopFromStack();
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
  return 0;
}
