#include "opcodes/INC.h"
#include "Processor6502.h"

INC::INC(Processor6502* cpu) : IOpcode(cpu) {}

bool INC::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched + 1;
  mCpu->Write(mCpu->addrAbs, temp);
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;
  return 0;
}
