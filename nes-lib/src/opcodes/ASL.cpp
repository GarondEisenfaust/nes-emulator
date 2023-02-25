#include "opcodes/ASL.h"
#include "Cpu.h"

ASL::ASL(Cpu* cpu) : IOpcode(cpu) {}

bool ASL::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->fetched << 1;
  mCpu->status.c = (temp & 0xFF00) > 0;
  mCpu->status.z = (temp & 0x00FF) == 0x00;
  mCpu->status.n = temp & 0x80;

  if (mCpu->lookup->at(mCpu->opcode).addrMode == &mCpu->addressingModes.imp) {
    mCpu->a = temp & 0x00FF;
  } else {
    mCpu->Write(mCpu->addrAbs, temp & 0x00FF);
  }
  return 0;
}

const char* ASL::Name() { return "ASL"; }
