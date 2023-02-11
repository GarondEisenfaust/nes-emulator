#include "opcodes/DEC.h"
#include "Processor6502.h"

DEC::DEC(Processor6502* cpu) : IOpcode(cpu) {}

bool DEC::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched - 1;
  mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
  return false;
}
