#include "opcodes/STX.h"
#include "Cpu.h"

STX::STX(Cpu* cpu) : IOpcode(cpu) {}

void STX::operator()() { mCpu->Write(mCpu->addrAbs, mCpu->x); }
