#include "opcodes/NOP.h"
#include "Cpu.h"

NOP::NOP(Cpu* cpu) : IOpcode(cpu) {}

bool NOP::operator()() {
  switch (mCpu->opcode) {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:
      return 1;
      break;
  }
  return 0;
}

const char* NOP::Name() { return "NOP"; }
