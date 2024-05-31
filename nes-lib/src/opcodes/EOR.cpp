#include "opcodes/EOR.h"
#include "Cpu.h"

EOR::EOR(Cpu* cpu) : IOpcode(cpu) {}

void EOR::operator()() {
  mCpu->Fetch();
  mCpu->a = mCpu->a ^ mCpu->fetched;
  mCpu->status.z = mCpu->a == 0;
  mCpu->status.n = mCpu->a & (1 << 7);
}

const char* EOR::Name() {
  return "EOR";
}
