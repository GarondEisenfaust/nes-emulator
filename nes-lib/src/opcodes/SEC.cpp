#include "opcodes/SEC.h"
#include "Cpu.h"

SEC::SEC(Cpu* cpu) : IOpcode(cpu) {}

void SEC::operator()() { mCpu->status.c = true; }
