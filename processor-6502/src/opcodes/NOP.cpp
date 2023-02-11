#include "opcodes/NOP.h"
#include "Processor6502.h"

NOP::NOP(Processor6502* cpu) : IOpcode(cpu) {}

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
