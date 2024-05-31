#include "opcodes/DEC.h"
#include "Cpu.h"

DEC::DEC(Cpu* cpu) : IOpcode(cpu) {}

void DEC::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched - 1;
  mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
}

const char* DEC::Name() {
  return "DEC";
}
