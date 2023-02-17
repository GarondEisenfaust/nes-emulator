#pragma once
#include <cstdint>

class IOpcode;
class IAddressingMode;

struct Instruction {
  IOpcode* opcode;
  IAddressingMode* addrMode;
  uint8_t cycles = 0;
};
