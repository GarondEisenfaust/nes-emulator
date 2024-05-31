#include "opcodes/LSR.h"
#include "Cpu.h"

LSR::LSR(Cpu* cpu) : IOpcode(cpu) {}

void LSR::operator()() {
  mCpu->Fetch();
  mCpu->status.c = mCpu->fetched & 0x0001;
  auto temp = mCpu->fetched >> 1;
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;

  if (mCpu->lookup->at(mCpu->opcode).addrMode == &mCpu->addressingModes.imp) {
    mCpu->a = temp & 0x00FF;
  } else {
    mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  }
}

const char* LSR::Name() {
  return "LSR";
}
