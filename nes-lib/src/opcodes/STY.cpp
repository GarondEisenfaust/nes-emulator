#include "opcodes/STY.h"
#include "Cpu.h"

STY::STY(Cpu* cpu) : IOpcode(cpu) {}

bool STY::operator()() {
  mCpu->Write(mCpu->addrAbs, mCpu->y);
  return false;
}

const char* STY::Name() { return "STY"; }
