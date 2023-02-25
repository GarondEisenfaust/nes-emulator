#include "opcodes/XXX.h"
#include "Cpu.h"

XXX::XXX(Cpu* cpu) : IOpcode(cpu) {}

bool XXX::operator()() { return false; }

const char* XXX::Name() { return "XXX"; }
