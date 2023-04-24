#include "opcodes/STY.h"
#include "Cpu.h"

STY::STY(Cpu* cpu) : IOpcode(cpu) {}

void STY::operator()() { mCpu->Write(mCpu->addrAbs, mCpu->y); }
