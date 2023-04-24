#include "opcodes/ROL.h"
#include "Cpu.h"

ROL::ROL(Cpu* cpu) : IOpcode(cpu) {}

void ROL::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched << 1 | mCpu->status.c;
  mCpu->status.c = temp & 0xFF00;
  mCpu->status.z = (temp & 0x00FF) == 0x0000;
  mCpu->status.n = temp & 0x0080;

  if (mCpu->lookup->at(mCpu->opcode).addrMode == &mCpu->addressingModes.imp) {
    mCpu->a = temp & 0x00FF;
  } else {
    mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  }
}
