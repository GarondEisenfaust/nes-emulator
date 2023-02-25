#include "opcodes/BIT.h"
#include "Cpu.h"

BIT::BIT(Cpu* cpu) : IOpcode(cpu) {}

bool BIT::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->a & mCpu->fetched;
  mCpu->status.z = (temp & 0x00FF) == 0x00;
  mCpu->status.n = mCpu->fetched & (1 << 7);
  mCpu->status.v = mCpu->fetched & (1 << 6);
  return 0;
}

const char* BIT::Name() { return "BIT"; }
