#include "opcodes/XXX.h"
#include "Cpu.h"

XXX::XXX(Cpu* cpu) : IOpcode(cpu) {}

void XXX::operator()() {}

const char* XXX::Name() {
  return "XXX";
}
