#include "opcodes/CPY.h"
#include "Cpu.h"

CPY::CPY(Cpu* cpu) : IOpcode(cpu) {}

void CPY::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->y - mCpu->fetched;
  mCpu->status.c = mCpu->y >= mCpu->fetched;
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
}
