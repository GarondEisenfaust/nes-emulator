#include "opcodes/EOR.h"
#include "Cpu.h"

EOR::EOR(Cpu* cpu) : IOpcode(cpu) {}

bool EOR::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a ^ mCpu->fetched;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
  return true;
}
