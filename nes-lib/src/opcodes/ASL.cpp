#include "opcodes/ASL.h"
#include "Cpu.h"

ASL::ASL(Cpu* cpu) : IOpcode(cpu) {}

void ASL::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched << 1;
  mCpu->status.c = (temp & 0xFF00) > 0;
  mCpu->status.z = (temp & 0x00FF) == 0;
  mCpu->status.n = temp & (1 << 7);

  if (mCpu->lookup->at(mCpu->opcode).addrMode == &mCpu->addressingModes.imp) {
    mCpu->a = temp & 0x00FF;
  } else {
    mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  }
}
