#include "opcodes/STA.h"
#include "Cpu.h"

STA::STA(Cpu* cpu) : IOpcode(cpu) {}

void STA::operator()() { mCpu->Write(mCpu->addrAbs, mCpu->a); }
