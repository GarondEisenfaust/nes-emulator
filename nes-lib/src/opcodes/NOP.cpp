#include "opcodes/NOP.h"
#include "Cpu.h"

NOP::NOP(Cpu* cpu) : IOpcode(cpu) {}

void NOP::operator()() {}

const char* NOP::Name() {
  return "NOP";
}
