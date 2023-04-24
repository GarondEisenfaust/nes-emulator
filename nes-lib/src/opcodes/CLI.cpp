#include "opcodes/CLI.h"
#include "Cpu.h"

CLI::CLI(Cpu* cpu) : IOpcode(cpu) {}

void CLI::operator()() { mCpu->status.i = false; }
