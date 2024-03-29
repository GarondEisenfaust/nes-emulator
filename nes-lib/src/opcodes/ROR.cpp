#include "opcodes/ROR.h"
#include "Cpu.h"

ROR::ROR(Cpu* cpu) : IOpcode(cpu) {}

void ROR::operator()() {
  mCpu->Fetch();

  auto temp = mCpu->status.c << 7 | (mCpu->fetched >> 1);
  mCpu->status.c = mCpu->fetched & 0x01;
  mCpu->status.z = (temp & 0x00FF) == 0x00;
  mCpu->status.n = temp & 0x0080;

  if (mCpu->lookup->at(mCpu->opcode).addrMode == &mCpu->addressingModes.imp) {
    mCpu->a = temp & 0x00FF;
  } else {
    mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  }
}
