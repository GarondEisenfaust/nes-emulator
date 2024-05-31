#include "opcodes/INC.h"
#include "Cpu.h"

INC::INC(Cpu* cpu) : IOpcode(cpu) {}

void INC::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched + 1;
  mCpu->Write(mCpu->addrAbs, temp);
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
}

const char* INC::Name() {
  return "INC";
}
