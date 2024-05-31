#include "opcodes/SBC.h"
#include "Cpu.h"

SBC::SBC(Cpu* cpu) : IOpcode(cpu) {}

void SBC::operator()() {
  mCpu->Fetch();

  uint16_t value = mCpu->fetched ^ 0x00FF;

  auto temp = mCpu->a + value + mCpu->status.c;
  mCpu->status.c = temp & 0xFF00;
  mCpu->status.z = (temp & 0x00FF) == 0;
  mCpu->status.v = (temp ^ mCpu->a) & (temp ^ value) & 0x0080;
  mCpu->status.n = temp & 0x0080;
  mCpu->a = temp & 0x00FF;
}

const char* SBC::Name() {
  return "SBC";
}
